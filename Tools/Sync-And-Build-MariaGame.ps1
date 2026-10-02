<#
.SYNOPSIS
    Safely updates MariaGame from GitHub, restores local work, builds the
    Unreal Engine project, extracts useful errors, and can launch the editor.

.DESCRIPTION
    This is the main MariaGame maintenance script.

    Workflow:
      1. Validate Git, repository, Unreal Engine and project paths.
      2. Detect branch/upstream and record the current commit.
      3. Protect local tracked/untracked changes in an automatic Git stash.
      4. Fetch origin and update with --ff-only.
      5. Restore the automatic stash. If conflicts occur, stop safely and keep
         the stash so nothing is lost.
      6. Run Git LFS pull when Git LFS is installed.
      7. Build MariaGameEditor.
      8. Parse the build log and write a concise LatestBuildErrors.txt report.
      9. Optionally launch Unreal Editor after a successful build.

    The script never performs git reset --hard, force checkout, force pull,
    or automatic rollback.

.VERSION
    2.0.0

.PREREQUISITES
    - Git
    - Unreal Engine 5.8
    - Visual Studio C++ toolchain required by Unreal Engine
    - Git LFS recommended before binary assets are added

.CHANGELOG
    2.0.0
      - Automatic safe stash/restore of local changes
      - Fetch + fast-forward-only update
      - Branch/upstream validation
      - Git LFS support
      - Full timestamped session/build logs
      - Compile-error extraction
      - Known-error diagnostics
      - Optional editor launch
      - Clear exit codes and checkpoints
    1.0.0
      - Initial wrapper around update/build scripts.
#>

[CmdletBinding()]
param(
    [string]$EngineRoot = 'I:\Spil\Epic Games\UE_5.8',

    [ValidateSet('Development','DebugGame')]
    [string]$Configuration = 'Development',

    [switch]$SkipBuild,
    [switch]$LaunchOnSuccess,
    [switch]$NoAutoStash,
    [switch]$NoRestoreStash
)

Set-StrictMode -Version 2.0
$ErrorActionPreference = 'Stop'

$ScriptVersion = '2.0.0'
$RepoRoot = Split-Path -Parent $PSScriptRoot
$Project = Join-Path $RepoRoot 'MariaGame.uproject'
$BuildBat = Join-Path $EngineRoot 'Engine\Build\BatchFiles\Build.bat'
$EditorExe = Join-Path $EngineRoot 'Engine\Binaries\Win64\UnrealEditor.exe'

$LogDir = Join-Path $PSScriptRoot 'BuildLogs'
$Timestamp = Get-Date -Format 'yyyyMMdd_HHmmss'
$SessionLog = Join-Path $LogDir ('SyncBuild_' + $Timestamp + '.log')
$BuildLog = Join-Path $LogDir ('Build_' + $Timestamp + '.log')
$LatestErrorReport = Join-Path $LogDir 'LatestBuildErrors.txt'

$AutoStashRef = $null
$AutoStashCreated = $false
$BeforeCommit = ''
$AfterCommit = ''
$Branch = ''
$Upstream = ''

# Exit codes:
#   0  Success
#   2  Local changes found and -NoAutoStash was used
#   10 Prerequisite/path failure
#   20 Git fetch/update failure
#   21 Branch has no upstream
#   22 Repository is in detached HEAD
#   23 Automatic stash could not be restored cleanly
#   30 Build failure
#   31 Unreal Editor is running and build reports Live Coding lock
#   40 Launch failure

New-Item -ItemType Directory -Path $LogDir -Force | Out-Null
Set-Content -Path $SessionLog -Value ('MariaGame Sync/Build log - ' + (Get-Date -Format 'yyyy-MM-dd HH:mm:ss'))

function Write-Log {
    param(
        [ValidateSet('INFO','OK','WARN','ERROR')]
        [string]$Level,
        [string]$Message
    )

    $Line = '[{0}] [{1}] {2}' -f (Get-Date -Format 'HH:mm:ss'), $Level, $Message
    Write-Host $Line
    Add-Content -Path $SessionLog -Value $Line
}

function Write-Checkpoint {
    param([string]$Name, [string]$Message)
    Write-Log -Level 'INFO' -Message ('CP:' + $Name + ' ' + $Message)
}

function Invoke-GitCmd {
    param(
        [Parameter(Mandatory=$true)]
        [string]$Arguments,

        [switch]$Quiet
    )

    $Command = 'git -C "{0}" {1}' -f $RepoRoot, $Arguments

    if (-not $Quiet) {
        Write-Log -Level 'INFO' -Message ('GIT> ' + $Arguments)
    }

    $Output = & cmd.exe /d /s /c $Command 2>&1
    $ExitCode = $LASTEXITCODE

    if (-not $Quiet -and $Output) {
        foreach ($Line in $Output) {
            Write-Host $Line
            Add-Content -Path $SessionLog -Value ([string]$Line)
        }
    }

    return [PSCustomObject]@{
        ExitCode = $ExitCode
        Output   = @($Output)
    }
}

function Get-GitSingleLine {
    param([string]$Arguments)

    $Result = Invoke-GitCmd -Arguments $Arguments -Quiet
    if ($Result.ExitCode -ne 0) {
        return $null
    }

    if ($Result.Output.Count -eq 0) {
        return ''
    }

    return ([string]$Result.Output[0]).Trim()
}

function Test-Prerequisites {
    Write-Checkpoint 'PREREQ' 'Checking required tools and paths.'

    $Git = Get-Command git -ErrorAction SilentlyContinue
    if (-not $Git) {
        Write-Log -Level 'ERROR' -Message 'Git was not found in PATH.'
        return $false
    }

    if (-not (Test-Path (Join-Path $RepoRoot '.git'))) {
        Write-Log -Level 'ERROR' -Message ('Git repository not found: ' + $RepoRoot)
        return $false
    }

    if (-not (Test-Path $Project)) {
        Write-Log -Level 'ERROR' -Message ('Project file not found: ' + $Project)
        return $false
    }

    if (-not $SkipBuild -and -not (Test-Path $BuildBat)) {
        Write-Log -Level 'ERROR' -Message ('Unreal Build.bat not found: ' + $BuildBat)
        return $false
    }

    Write-Log -Level 'OK' -Message ('Repository: ' + $RepoRoot)
    Write-Log -Level 'OK' -Message ('Project: ' + $Project)

    if (-not $SkipBuild) {
        Write-Log -Level 'OK' -Message ('Engine: ' + $EngineRoot)
    }

    return $true
}

function Get-WorkingTreeChanges {
    $Result = Invoke-GitCmd -Arguments 'status --porcelain' -Quiet

    if ($Result.ExitCode -ne 0) {
        throw 'git status failed.'
    }

    return @($Result.Output | Where-Object { -not [string]::IsNullOrWhiteSpace([string]$_) })
}

function Protect-LocalChanges {
    $Changes = Get-WorkingTreeChanges

    if ($Changes.Count -eq 0) {
        Write-Log -Level 'OK' -Message 'Working tree is clean.'
        return $true
    }

    Write-Log -Level 'WARN' -Message ('Local changes detected: ' + $Changes.Count)

    foreach ($Line in $Changes) {
        Write-Host ('  ' + [string]$Line)
        Add-Content -Path $SessionLog -Value ('  ' + [string]$Line)
    }

    if ($NoAutoStash) {
        Write-Log -Level 'ERROR' -Message 'Update stopped because -NoAutoStash was specified.'
        return $false
    }

    $StashMessage = 'MariaGame AutoStash ' + (Get-Date -Format 'yyyy-MM-dd HH:mm:ss')
    Write-Checkpoint 'STASH' ('Protecting local changes as "' + $StashMessage + '".')

    $Result = Invoke-GitCmd -Arguments ('stash push --include-untracked -m "' + $StashMessage + '"')
    if ($Result.ExitCode -ne 0) {
        Write-Log -Level 'ERROR' -Message 'Unable to create automatic stash.'
        return $false
    }

    $script:AutoStashRef = Get-GitSingleLine 'stash list -1 --format="%gd"'
    if ([string]::IsNullOrWhiteSpace($script:AutoStashRef)) {
        Write-Log -Level 'ERROR' -Message 'Git reported changes, but no stash reference could be determined.'
        return $false
    }

    $script:AutoStashCreated = $true
    Write-Log -Level 'OK' -Message ('Local work protected in ' + $script:AutoStashRef)
    return $true
}

function Restore-LocalChanges {
    if (-not $script:AutoStashCreated) {
        return $true
    }

    if ($NoRestoreStash) {
        Write-Log -Level 'WARN' -Message ('Local changes remain safely stored in ' + $script:AutoStashRef)
        return $true
    }

    Write-Checkpoint 'STASH-RESTORE' ('Applying ' + $script:AutoStashRef)

    $Apply = Invoke-GitCmd -Arguments ('stash apply ' + $script:AutoStashRef)
    if ($Apply.ExitCode -ne 0) {
        Write-Log -Level 'ERROR' -Message 'Automatic stash restore produced conflicts.'
        Write-Log -Level 'WARN' -Message ('The stash was NOT deleted and remains available as ' + $script:AutoStashRef)

        $Conflicts = Invoke-GitCmd -Arguments 'diff --name-only --diff-filter=U' -Quiet
        if ($Conflicts.Output.Count -gt 0) {
            Write-Host ''
            Write-Host 'Conflicting files:'
            foreach ($File in $Conflicts.Output) {
                if (-not [string]::IsNullOrWhiteSpace([string]$File)) {
                    Write-Host ('  ' + [string]$File)
                    Add-Content -Path $SessionLog -Value ('CONFLICT: ' + [string]$File)
                }
            }
        }

        return $false
    }

    $Drop = Invoke-GitCmd -Arguments ('stash drop ' + $script:AutoStashRef)
    if ($Drop.ExitCode -ne 0) {
        Write-Log -Level 'WARN' -Message 'Changes were restored, but automatic stash cleanup failed. Review git stash list.'
        return $true
    }

    Write-Log -Level 'OK' -Message 'Local changes restored successfully.'
    return $true
}

function Update-Repository {
    Write-Checkpoint 'GIT' 'Inspecting branch and upstream.'

    $script:Branch = Get-GitSingleLine 'branch --show-current'
    if ([string]::IsNullOrWhiteSpace($script:Branch)) {
        Write-Log -Level 'ERROR' -Message 'Repository is in detached HEAD state. Update aborted.'
        exit 22
    }

    $script:Upstream = Get-GitSingleLine 'rev-parse --abbrev-ref --symbolic-full-name "@{u}"'
    if ([string]::IsNullOrWhiteSpace($script:Upstream)) {
        Write-Log -Level 'ERROR' -Message ('Branch "' + $script:Branch + '" has no upstream tracking branch.')
        exit 21
    }

    $script:BeforeCommit = Get-GitSingleLine 'rev-parse --short HEAD'

    Write-Log -Level 'INFO' -Message ('Branch: ' + $script:Branch)
    Write-Log -Level 'INFO' -Message ('Upstream: ' + $script:Upstream)
    Write-Log -Level 'INFO' -Message ('Before: ' + $script:BeforeCommit)

    $Fetch = Invoke-GitCmd -Arguments 'fetch --prune origin'
    if ($Fetch.ExitCode -ne 0) {
        Write-Log -Level 'ERROR' -Message 'git fetch failed.'
        return $false
    }

    Write-Checkpoint 'PULL' 'Updating with --ff-only. No force/reset operations are used.'
    $Pull = Invoke-GitCmd -Arguments 'pull --ff-only'
    if ($Pull.ExitCode -ne 0) {
        Write-Log -Level 'ERROR' -Message 'git pull --ff-only failed.'
        Write-Log -Level 'WARN' -Message 'The branch may have diverged. No reset or force operation was attempted.'
        return $false
    }

    $script:AfterCommit = Get-GitSingleLine 'rev-parse --short HEAD'

    if ($script:BeforeCommit -eq $script:AfterCommit) {
        Write-Log -Level 'OK' -Message ('Already current at ' + $script:AfterCommit)
    }
    else {
        Write-Log -Level 'OK' -Message ('Updated ' + $script:BeforeCommit + ' -> ' + $script:AfterCommit)
    }

    return $true
}

function Update-GitLfs {
    $LfsVersion = & git lfs version 2>$null

    if ($LASTEXITCODE -ne 0) {
        Write-Log -Level 'WARN' -Message 'Git LFS is not installed. Text/code updates still work, but future binary assets may not.'
        return
    }

    Write-Checkpoint 'LFS' ([string]($LfsVersion -join ' '))

    $Install = Invoke-GitCmd -Arguments 'lfs install --local'
    if ($Install.ExitCode -ne 0) {
        Write-Log -Level 'WARN' -Message 'git lfs install --local failed.'
        return
    }

    $Pull = Invoke-GitCmd -Arguments 'lfs pull'
    if ($Pull.ExitCode -ne 0) {
        Write-Log -Level 'WARN' -Message 'git lfs pull failed. Code can still build if no required LFS assets are missing.'
    }
}

function Get-BuildErrors {
    param([string]$Path)

    if (-not (Test-Path $Path)) {
        return @()
    }

    $Patterns = @(
        '\berror C\d+\b',
        '\berror CS\d+\b',
        '\bfatal error\b',
        'UnrealHeaderTool.*(error|failed)',
        '\berror:\s',
        'Result:\s*Failed',
        'OtherCompilationError',
        'Unable to build while Live Coding is active'
    )

    $Lines = Get-Content -Path $Path
    $Matches = New-Object System.Collections.Generic.List[string]

    foreach ($Line in $Lines) {
        foreach ($Pattern in $Patterns) {
            if ($Line -match $Pattern) {
                $Matches.Add($Line)
                break
            }
        }
    }

    return @($Matches | Select-Object -Unique)
}

function Write-BuildDiagnostics {
    param(
        [string]$Path,
        [int]$ExitCode
    )

    $Errors = Get-BuildErrors -Path $Path

    $Report = New-Object System.Collections.Generic.List[string]
    $Report.Add('MariaGame latest build error report')
    $Report.Add('Generated: ' + (Get-Date -Format 'yyyy-MM-dd HH:mm:ss'))
    $Report.Add('Commit: ' + $script:AfterCommit)
    $Report.Add('Exit code: ' + $ExitCode)
    $Report.Add('Build log: ' + $Path)
    $Report.Add('')

    if ($Errors.Count -eq 0) {
        $Report.Add('No standard compiler error pattern was detected. Review the full build log.')
    }
    else {
        $Report.Add('Detected errors:')
        foreach ($ErrorLine in $Errors) {
            $Report.Add([string]$ErrorLine)
        }
    }

    $Report | Set-Content -Path $LatestErrorReport

    Write-Host ''
    Write-Host '===== BUILD ERROR SUMMARY ====='

    if ($Errors.Count -eq 0) {
        Write-Host 'No standard compile-error line was detected.'
        Write-Host ('Full log: ' + $Path)
    }
    else {
        $MaxErrors = [Math]::Min(20, $Errors.Count)
        for ($Index = 0; $Index -lt $MaxErrors; $Index++) {
            Write-Host ([string]$Errors[$Index])
        }

        if ($Errors.Count -gt $MaxErrors) {
            Write-Host ('... plus ' + ($Errors.Count - $MaxErrors) + ' additional matched lines.')
        }
    }

    Write-Host ('Error report: ' + $LatestErrorReport)
    Write-Host '==============================='
    Write-Host ''

    $FullText = ''
    if (Test-Path $Path) {
        $FullText = Get-Content -Path $Path -Raw
    }

    if ($FullText -match 'Unable to build while Live Coding is active') {
        Write-Log -Level 'ERROR' -Message 'Known cause: Unreal Live Coding is active. Close the editor or disable Live Coding, then run the script again.'
        return 31
    }

    if ($FullText -match 'Visual Studio compiler version .* newer than latest preferred version') {
        Write-Log -Level 'WARN' -Message 'Your MSVC toolchain is newer than Unreal preferred. This is a warning unless another actual error is present.'
    }

    if ($FullText -match 'could not be compiled.*rebuilding from source manually') {
        Write-Log -Level 'WARN' -Message 'Editor module load failed. The extracted compiler errors above are the useful root cause.'
    }

    return 30
}

function Build-Project {
    if ($SkipBuild) {
        Write-Log -Level 'WARN' -Message 'Build skipped by -SkipBuild.'
        return 0
    }

    $RunningEditor = Get-Process -Name 'UnrealEditor' -ErrorAction SilentlyContinue
    if ($RunningEditor) {
        Write-Log -Level 'WARN' -Message 'UnrealEditor is currently running. A normal build may fail if Live Coding is active.'
    }

    Write-Checkpoint 'BUILD' ('MariaGameEditor Win64 ' + $Configuration)
    Set-Content -Path $BuildLog -Value ('MariaGame build log - ' + (Get-Date -Format 'yyyy-MM-dd HH:mm:ss'))

    $BuildCommand = '"{0}" MariaGameEditor Win64 {1} -Project="{2}" -WaitMutex' -f $BuildBat, $Configuration, $Project

    Write-Log -Level 'INFO' -Message ('BUILD> ' + $BuildCommand)

    $Output = & cmd.exe /d /s /c $BuildCommand 2>&1
    $ExitCode = $LASTEXITCODE

    foreach ($Line in $Output) {
        Write-Host $Line
        Add-Content -Path $BuildLog -Value ([string]$Line)
    }

    if ($ExitCode -eq 0) {
        Write-Log -Level 'OK' -Message 'Unreal build succeeded.'
        if (Test-Path $LatestErrorReport) {
            Remove-Item $LatestErrorReport -Force -ErrorAction SilentlyContinue
        }
        return 0
    }

    Write-Log -Level 'ERROR' -Message ('Unreal build failed with exit code ' + $ExitCode)
    return (Write-BuildDiagnostics -Path $BuildLog -ExitCode $ExitCode)
}

function Launch-Editor {
    if (-not $LaunchOnSuccess) {
        return $true
    }

    if (-not (Test-Path $EditorExe)) {
        Write-Log -Level 'ERROR' -Message ('UnrealEditor.exe not found: ' + $EditorExe)
        return $false
    }

    Write-Checkpoint 'LAUNCH' 'Starting Unreal Editor.'
    try {
        Start-Process -FilePath $EditorExe -ArgumentList @($Project)
        Write-Log -Level 'OK' -Message 'Unreal Editor launched.'
        return $true
    }
    catch {
        Write-Log -Level 'ERROR' -Message ('Unable to launch Unreal Editor: ' + $_.Exception.Message)
        return $false
    }
}

try {
    Write-Checkpoint 'START' ('Sync-And-Build-MariaGame.ps1 v' + $ScriptVersion)

    if (-not (Test-Prerequisites)) {
        exit 10
    }

    if (-not (Protect-LocalChanges)) {
        exit 2
    }

    if (-not (Update-Repository)) {
        Write-Log -Level 'ERROR' -Message 'Repository update failed.'

        if ($AutoStashCreated -and -not $NoRestoreStash) {
            Write-Log -Level 'INFO' -Message 'Attempting to restore local changes because the Git update failed.'
            [void](Restore-LocalChanges)
        }

        exit 20
    }

    Update-GitLfs

    if (-not (Restore-LocalChanges)) {
        exit 23
    }

    Write-Checkpoint 'STATUS' 'Repository state before build.'
    $Status = Invoke-GitCmd -Arguments 'status --short --branch'
    if ($Status.ExitCode -ne 0) {
        Write-Log -Level 'WARN' -Message 'Unable to display final Git status.'
    }

    $BuildResult = Build-Project
    if ($BuildResult -ne 0) {
        Write-Host ''
        Write-Log -Level 'ERROR' -Message 'SYNC succeeded, BUILD failed.'
        Write-Log -Level 'INFO' -Message ('Session log: ' + $SessionLog)
        Write-Log -Level 'INFO' -Message ('Build log: ' + $BuildLog)
        exit $BuildResult
    }

    if (-not (Launch-Editor)) {
        exit 40
    }

    Write-Host ''
    Write-Log -Level 'OK' -Message 'SYNC + BUILD completed successfully.'
    Write-Log -Level 'INFO' -Message ('Commit: ' + $AfterCommit)
    Write-Log -Level 'INFO' -Message ('Session log: ' + $SessionLog)

    if (-not $SkipBuild) {
        Write-Log -Level 'INFO' -Message ('Build log: ' + $BuildLog)
    }

    exit 0
}
catch {
    Write-Log -Level 'ERROR' -Message ('Unhandled error: ' + $_.Exception.Message)
    Write-Log -Level 'ERROR' -Message ('At: ' + $_.InvocationInfo.PositionMessage)
    Write-Log -Level 'INFO' -Message ('Session log: ' + $SessionLog)
    exit 99
}
