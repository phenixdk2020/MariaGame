<#
.SYNOPSIS
    Safely updates MariaGame from GitHub, builds Unreal, extracts compile
    errors, and optionally starts Unreal Editor.

.VERSION
    2.1.1

.DESCRIPTION
    Safe daily workflow for MariaGame.

    Key rules:
      - Logs live in Saved\BuildLogs (Git-ignored).
      - Only tracked local changes are auto-stashed.
      - Untracked files are NEVER removed by the script.
      - Incoming files are checked against local untracked files before pull.
      - Pull uses --ff-only.
      - Existing old MariaGame AutoStash entries are reported but never touched.
      - No reset --hard, force checkout, force pull or destructive cleanup.

.CHANGELOG
    2.1.1
      - Git is executed through System.Diagnostics.Process
      - Normal Git stderr progress no longer becomes PowerShell NativeCommandError
    2.1.0
      - Fixed v2.0 bug where Tools\BuildLogs was stashed while in use
      - Logs moved to Saved\BuildLogs
      - Untracked files are no longer included in auto-stash
      - Added untracked-vs-incoming collision detection
      - Logging is resilient if a log write fails
      - Existing interrupted AutoStash entries are preserved and reported
    2.0.0
      - Safe Git sync, build error extraction and optional launch
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

$ScriptVersion = '2.1.1'
$RepoRoot = Split-Path -Parent $PSScriptRoot
$Project = Join-Path $RepoRoot 'MariaGame.uproject'
$BuildBat = Join-Path $EngineRoot 'Engine\Build\BatchFiles\Build.bat'
$EditorExe = Join-Path $EngineRoot 'Engine\Binaries\Win64\UnrealEditor.exe'

$LogDir = Join-Path $RepoRoot 'Saved\BuildLogs'
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

function Ensure-LogDirectory {
    if (-not (Test-Path $LogDir)) {
        New-Item -ItemType Directory -Path $LogDir -Force | Out-Null
    }
}

function Write-Log {
    param(
        [ValidateSet('INFO','OK','WARN','ERROR')]
        [string]$Level,
        [string]$Message
    )

    $Line = '[{0}] [{1}] {2}' -f (Get-Date -Format 'HH:mm:ss'), $Level, $Message
    Write-Host $Line

    try {
        Ensure-LogDirectory
        Add-Content -Path $SessionLog -Value $Line -ErrorAction Stop
    }
    catch {
        Write-Host ('[LOG-WARN] ' + $_.Exception.Message)
    }
}

function Write-Checkpoint {
    param([string]$Name, [string]$Message)
    Write-Log -Level INFO -Message ('CP:' + $Name + ' ' + $Message)
}

function Add-SessionText {
    param([string]$Text)
    try {
        Ensure-LogDirectory
        Add-Content -Path $SessionLog -Value $Text -ErrorAction Stop
    }
    catch {
        Write-Host ('[LOG-WARN] ' + $_.Exception.Message)
    }
}

function Invoke-GitCmd {
    param(
        [Parameter(Mandatory=$true)]
        [string]$Arguments,
        [switch]$Quiet
    )

    if (-not $Quiet) {
        Write-Log -Level INFO -Message ('GIT> ' + $Arguments)
    }

    $GitPath = (Get-Command git -ErrorAction Stop).Source

    $StartInfo = New-Object System.Diagnostics.ProcessStartInfo
    $StartInfo.FileName = $GitPath
    $StartInfo.Arguments = '-C "' + $RepoRoot + '" ' + $Arguments
    $StartInfo.UseShellExecute = $false
    $StartInfo.RedirectStandardOutput = $true
    $StartInfo.RedirectStandardError = $true
    $StartInfo.CreateNoWindow = $true

    $Process = New-Object System.Diagnostics.Process
    $Process.StartInfo = $StartInfo

    [void]$Process.Start()

    $StdOut = $Process.StandardOutput.ReadToEnd()
    $StdErr = $Process.StandardError.ReadToEnd()
    $Process.WaitForExit()

    $Code = $Process.ExitCode
    $Lines = New-Object System.Collections.Generic.List[string]

    if (-not [string]::IsNullOrWhiteSpace($StdOut)) {
        foreach ($Line in ($StdOut -split "\r?\n")) {
            if (-not [string]::IsNullOrWhiteSpace($Line)) {
                $Lines.Add($Line)
            }
        }
    }

    if (-not [string]::IsNullOrWhiteSpace($StdErr)) {
        foreach ($Line in ($StdErr -split "\r?\n")) {
            if (-not [string]::IsNullOrWhiteSpace($Line)) {
                $Lines.Add($Line)
            }
        }
    }

    if (-not $Quiet) {
        foreach ($Line in $Lines) {
            Write-Host $Line
            Add-SessionText $Line
        }
    }

    [PSCustomObject]@{
        ExitCode = $Code
        Output   = @($Lines)
    }
}

function Get-GitSingleLine {
    param([string]$Arguments)

    $Result = Invoke-GitCmd -Arguments $Arguments -Quiet
    if ($Result.ExitCode -ne 0 -or $Result.Output.Count -eq 0) {
        return ''
    }

    ([string]$Result.Output[0]).Trim()
}

function Get-TrackedChanges {
    $Result = Invoke-GitCmd -Arguments 'status --porcelain --untracked-files=no' -Quiet
    if ($Result.ExitCode -ne 0) {
        throw 'git status failed.'
    }

    @(
        $Result.Output |
        ForEach-Object { [string]$_ } |
        Where-Object { -not [string]::IsNullOrWhiteSpace($_) }
    )
}

function Get-UntrackedFiles {
    $Result = Invoke-GitCmd -Arguments 'ls-files --others --exclude-standard' -Quiet
    if ($Result.ExitCode -ne 0) {
        throw 'git ls-files failed.'
    }

    @(
        $Result.Output |
        ForEach-Object { ([string]$_).Trim().Replace('\','/') } |
        Where-Object { -not [string]::IsNullOrWhiteSpace($_) }
    )
}

function Test-Prerequisites {
    Write-Checkpoint PREREQ 'Checking Git, repository, project and Unreal paths.'

    if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
        Write-Log -Level ERROR -Message 'Git was not found in PATH.'
        return $false
    }

    if (-not (Test-Path (Join-Path $RepoRoot '.git'))) {
        Write-Log -Level ERROR -Message ('Git repository not found: ' + $RepoRoot)
        return $false
    }

    if (-not (Test-Path $Project)) {
        Write-Log -Level ERROR -Message ('Project file not found: ' + $Project)
        return $false
    }

    if (-not $SkipBuild -and -not (Test-Path $BuildBat)) {
        Write-Log -Level ERROR -Message ('Build.bat not found: ' + $BuildBat)
        return $false
    }

    Write-Log -Level OK -Message ('Repository: ' + $RepoRoot)
    Write-Log -Level OK -Message ('Project: ' + $Project)

    if (-not $SkipBuild) {
        Write-Log -Level OK -Message ('Engine: ' + $EngineRoot)
    }

    return $true
}

function Show-ExistingAutoStashes {
    $Result = Invoke-GitCmd -Arguments 'stash list --format="%gd|%gs"' -Quiet
    if ($Result.ExitCode -ne 0) {
        return
    }

    $Old = @(
        $Result.Output |
        ForEach-Object { [string]$_ } |
        Where-Object { $_ -match 'MariaGame AutoStash' }
    )

    if ($Old.Count -gt 0) {
        Write-Log -Level WARN -Message ('Existing MariaGame AutoStash entries: ' + $Old.Count)
        Write-Log -Level WARN -Message 'They may contain work from an earlier interrupted run. They will NOT be modified.'

        foreach ($Entry in $Old) {
            Write-Host ('  ' + $Entry)
        }
    }
}

function Protect-TrackedChanges {
    $Tracked = @(Get-TrackedChanges)
    $Untracked = @(Get-UntrackedFiles)

    if ($Tracked.Count -eq 0) {
        Write-Log -Level OK -Message 'No tracked local changes.'
    }
    else {
        Write-Log -Level WARN -Message ('Tracked local changes detected: ' + $Tracked.Count)
        foreach ($Line in $Tracked) {
            Write-Host ('  ' + $Line)
        }

        if ($NoAutoStash) {
            Write-Log -Level ERROR -Message 'Stopped because -NoAutoStash was specified.'
            return $false
        }

        $Message = 'MariaGame AutoStash ' + (Get-Date -Format 'yyyy-MM-dd HH:mm:ss')
        Write-Checkpoint STASH ('Protecting tracked changes as "' + $Message + '".')

        # Deliberately excludes untracked files.
        $Result = Invoke-GitCmd -Arguments ('stash push -m "' + $Message + '"')
        if ($Result.ExitCode -ne 0) {
            Write-Log -Level ERROR -Message 'Unable to stash tracked local changes.'
            return $false
        }

        $script:AutoStashRef = Get-GitSingleLine 'stash list -1 --format="%gd"'
        if ([string]::IsNullOrWhiteSpace($script:AutoStashRef)) {
            Write-Log -Level ERROR -Message 'A stash was expected but its reference could not be determined.'
            return $false
        }

        $script:AutoStashCreated = $true
        Write-Log -Level OK -Message ('Tracked work protected in ' + $script:AutoStashRef)
    }

    if ($Untracked.Count -gt 0) {
        Write-Log -Level INFO -Message ('Untracked files left untouched: ' + $Untracked.Count)
        foreach ($Path in ($Untracked | Select-Object -First 10)) {
            Write-Host ('  ?? ' + $Path)
        }
        if ($Untracked.Count -gt 10) {
            Write-Host ('  ... plus ' + ($Untracked.Count - 10) + ' more')
        }
    }

    return $true
}

function Restore-TrackedChanges {
    if (-not $script:AutoStashCreated) {
        return $true
    }

    if ($NoRestoreStash) {
        Write-Log -Level WARN -Message ('Tracked changes remain safely in ' + $script:AutoStashRef)
        return $true
    }

    Write-Checkpoint STASH-RESTORE ('Applying ' + $script:AutoStashRef)
    $Apply = Invoke-GitCmd -Arguments ('stash apply ' + $script:AutoStashRef)

    if ($Apply.ExitCode -ne 0) {
        Write-Log -Level ERROR -Message 'Restore produced conflicts.'
        Write-Log -Level WARN -Message ('Stash preserved: ' + $script:AutoStashRef)

        $Conflicts = Invoke-GitCmd -Arguments 'diff --name-only --diff-filter=U' -Quiet
        foreach ($File in $Conflicts.Output) {
            if (-not [string]::IsNullOrWhiteSpace([string]$File)) {
                Write-Host ('  CONFLICT: ' + [string]$File)
            }
        }

        return $false
    }

    $Drop = Invoke-GitCmd -Arguments ('stash drop ' + $script:AutoStashRef)
    if ($Drop.ExitCode -ne 0) {
        Write-Log -Level WARN -Message 'Changes were restored but stash cleanup failed. Review git stash list.'
        return $true
    }

    Write-Log -Level OK -Message 'Tracked local changes restored.'
    return $true
}

function Test-UntrackedIncomingCollision {
    $Untracked = @(Get-UntrackedFiles)
    if ($Untracked.Count -eq 0) {
        return $true
    }

    $Diff = Invoke-GitCmd -Arguments ('diff --name-only HEAD..' + $script:Upstream + ' --') -Quiet
    if ($Diff.ExitCode -ne 0) {
        Write-Log -Level WARN -Message 'Unable to pre-check untracked files against incoming paths.'
        return $true
    }

    $Incoming = @(
        $Diff.Output |
        ForEach-Object { ([string]$_).Trim().Replace('\','/') } |
        Where-Object { -not [string]::IsNullOrWhiteSpace($_) }
    )

    $Collisions = @($Untracked | Where-Object { $Incoming -contains $_ })

    if ($Collisions.Count -eq 0) {
        return $true
    }

    Write-Log -Level ERROR -Message 'Local untracked files would be overwritten by the incoming Git update:'
    foreach ($Path in $Collisions) {
        Write-Host ('  ' + $Path)
    }

    Write-Log -Level WARN -Message 'Move/rename those files manually. The script will not delete them.'
    return $false
}

function Update-Repository {
    Write-Checkpoint GIT 'Inspecting branch and upstream.'

    $script:Branch = Get-GitSingleLine 'branch --show-current'
    if ([string]::IsNullOrWhiteSpace($script:Branch)) {
        Write-Log -Level ERROR -Message 'Detached HEAD. Update aborted.'
        return 22
    }

    $script:Upstream = Get-GitSingleLine 'rev-parse --abbrev-ref --symbolic-full-name "@{u}"'
    if ([string]::IsNullOrWhiteSpace($script:Upstream)) {
        Write-Log -Level ERROR -Message ('Branch "' + $script:Branch + '" has no upstream.')
        return 21
    }

    $script:BeforeCommit = Get-GitSingleLine 'rev-parse --short HEAD'

    Write-Log -Level INFO -Message ('Branch: ' + $script:Branch)
    Write-Log -Level INFO -Message ('Upstream: ' + $script:Upstream)
    Write-Log -Level INFO -Message ('Before: ' + $script:BeforeCommit)

    Write-Checkpoint FETCH 'Fetching origin.'
    $Fetch = Invoke-GitCmd -Arguments 'fetch --prune origin'
    if ($Fetch.ExitCode -ne 0) {
        Write-Log -Level ERROR -Message 'git fetch failed.'
        return 20
    }

    if (-not (Test-UntrackedIncomingCollision)) {
        return 24
    }

    Write-Checkpoint PULL 'Fast-forward-only update.'
    $Pull = Invoke-GitCmd -Arguments 'pull --ff-only'
    if ($Pull.ExitCode -ne 0) {
        Write-Log -Level ERROR -Message 'git pull --ff-only failed.'
        Write-Log -Level WARN -Message 'No reset/rebase/force action was attempted.'
        return 20
    }

    $script:AfterCommit = Get-GitSingleLine 'rev-parse --short HEAD'

    if ($script:BeforeCommit -eq $script:AfterCommit) {
        Write-Log -Level OK -Message ('Already current at ' + $script:AfterCommit)
    }
    else {
        Write-Log -Level OK -Message ('Updated ' + $script:BeforeCommit + ' -> ' + $script:AfterCommit)
    }

    return 0
}

function Update-GitLfs {
    $Version = & git lfs version 2>$null
    if ($LASTEXITCODE -ne 0) {
        Write-Log -Level WARN -Message 'Git LFS is not installed. Future binary assets may require it.'
        return
    }

    Write-Checkpoint LFS ([string]($Version -join ' '))

    $Install = Invoke-GitCmd -Arguments 'lfs install --local'
    if ($Install.ExitCode -ne 0) {
        Write-Log -Level WARN -Message 'git lfs install --local failed.'
        return
    }

    $Pull = Invoke-GitCmd -Arguments 'lfs pull'
    if ($Pull.ExitCode -ne 0) {
        Write-Log -Level WARN -Message 'git lfs pull failed.'
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

    $Matches = New-Object System.Collections.Generic.List[string]

    foreach ($Line in (Get-Content -Path $Path)) {
        foreach ($Pattern in $Patterns) {
            if ($Line -match $Pattern) {
                $Matches.Add([string]$Line)
                break
            }
        }
    }

    @($Matches | Select-Object -Unique)
}

function Write-BuildDiagnostics {
    param([string]$Path, [int]$ExitCode)

    $Errors = Get-BuildErrors -Path $Path

    $Report = @(
        'MariaGame latest build error report',
        ('Generated: ' + (Get-Date -Format 'yyyy-MM-dd HH:mm:ss')),
        ('Commit: ' + $script:AfterCommit),
        ('Exit code: ' + $ExitCode),
        ('Build log: ' + $Path),
        ''
    )

    if ($Errors.Count -eq 0) {
        $Report += 'No standard compiler-error pattern detected. Review the full build log.'
    }
    else {
        $Report += 'Detected errors:'
        $Report += $Errors
    }

    Ensure-LogDirectory
    $Report | Set-Content -Path $LatestErrorReport

    Write-Host ''
    Write-Host '===== BUILD ERROR SUMMARY ====='

    if ($Errors.Count -eq 0) {
        Write-Host 'No standard compiler-error line detected.'
    }
    else {
        $Errors | Select-Object -First 20 | ForEach-Object { Write-Host $_ }
    }

    Write-Host ('Full log:   ' + $Path)
    Write-Host ('Error file: ' + $LatestErrorReport)
    Write-Host '==============================='

    $FullText = Get-Content -Path $Path -Raw

    if ($FullText -match 'Unable to build while Live Coding is active') {
        Write-Log -Level ERROR -Message 'Live Coding is active. Close Unreal Editor or disable Live Coding, then retry.'
        return 31
    }

    if ($FullText -match 'Visual Studio compiler version .* newer than latest preferred version') {
        Write-Log -Level WARN -Message 'MSVC is newer than Unreal preferred. This is only a warning unless another error is shown.'
    }

    return 30
}

function Build-Project {
    if ($SkipBuild) {
        Write-Log -Level WARN -Message 'Build skipped.'
        return 0
    }

    if (Get-Process -Name UnrealEditor -ErrorAction SilentlyContinue) {
        Write-Log -Level WARN -Message 'UnrealEditor is running. Close it if Live Coding blocks the build.'
    }

    Ensure-LogDirectory
    Set-Content -Path $BuildLog -Value ('MariaGame build log - ' + (Get-Date -Format 'yyyy-MM-dd HH:mm:ss'))

    Write-Checkpoint BUILD ('MariaGameEditor Win64 ' + $Configuration)

    $Command = '"{0}" MariaGameEditor Win64 {1} -Project="{2}" -WaitMutex' -f $BuildBat, $Configuration, $Project
    Write-Log -Level INFO -Message ('BUILD> ' + $Command)

    $Output = & cmd.exe /d /s /c $Command 2>&1
    $Code = $LASTEXITCODE

    foreach ($Line in $Output) {
        Write-Host $Line
        Add-Content -Path $BuildLog -Value ([string]$Line)
    }

    if ($Code -eq 0) {
        Write-Log -Level OK -Message 'Unreal build succeeded.'
        Remove-Item $LatestErrorReport -Force -ErrorAction SilentlyContinue
        return 0
    }

    Write-Log -Level ERROR -Message ('Unreal build failed with exit code ' + $Code)
    return (Write-BuildDiagnostics -Path $BuildLog -ExitCode $Code)
}

function Launch-Editor {
    if (-not $LaunchOnSuccess) {
        return $true
    }

    if (-not (Test-Path $EditorExe)) {
        Write-Log -Level ERROR -Message ('UnrealEditor.exe not found: ' + $EditorExe)
        return $false
    }

    Write-Checkpoint LAUNCH 'Starting Unreal Editor.'
    Start-Process -FilePath $EditorExe -ArgumentList @($Project)
    Write-Log -Level OK -Message 'Unreal Editor launched.'
    return $true
}

Ensure-LogDirectory
Set-Content -Path $SessionLog -Value ('MariaGame Sync/Build v' + $ScriptVersion + ' - ' + (Get-Date -Format 'yyyy-MM-dd HH:mm:ss'))

try {
    Write-Checkpoint START ('Sync-And-Build-MariaGame.ps1 v' + $ScriptVersion)

    if (-not (Test-Prerequisites)) {
        exit 10
    }

    Show-ExistingAutoStashes

    if (-not (Protect-TrackedChanges)) {
        exit 2
    }

    $UpdateResult = Update-Repository

    if ($UpdateResult -ne 0) {
        if ($AutoStashCreated -and -not $NoRestoreStash) {
            Write-Log -Level INFO -Message 'Git update did not complete. Restoring this run''s tracked-change stash.'
            [void](Restore-TrackedChanges)
        }

        exit $UpdateResult
    }

    Update-GitLfs

    if (-not (Restore-TrackedChanges)) {
        exit 23
    }

    Write-Checkpoint STATUS 'Repository state before build.'
    [void](Invoke-GitCmd -Arguments 'status --short --branch')

    $BuildResult = Build-Project
    if ($BuildResult -ne 0) {
        Write-Log -Level ERROR -Message 'SYNC succeeded, BUILD failed.'
        Write-Log -Level INFO -Message ('Session log: ' + $SessionLog)
        Write-Log -Level INFO -Message ('Build log: ' + $BuildLog)
        exit $BuildResult
    }

    if (-not (Launch-Editor)) {
        exit 40
    }

    Write-Host ''
    Write-Log -Level OK -Message 'SYNC + BUILD completed successfully.'
    Write-Log -Level INFO -Message ('Commit: ' + $AfterCommit)
    Write-Log -Level INFO -Message ('Session log: ' + $SessionLog)

    if (-not $SkipBuild) {
        Write-Log -Level INFO -Message ('Build log: ' + $BuildLog)
    }

    exit 0
}
catch {
    Write-Log -Level ERROR -Message ('Unhandled error: ' + $_.Exception.Message)
    Write-Log -Level ERROR -Message ('At: ' + $_.InvocationInfo.PositionMessage)
    Write-Log -Level INFO -Message ('Session log: ' + $SessionLog)
    exit 99
}



