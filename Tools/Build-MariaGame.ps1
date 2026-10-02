<#
.SYNOPSIS
    Builds MariaGameEditor for Unreal Engine 5.8.

.VERSION
    1.0.0

.CHANGELOG
    1.0.0 - Initial UE 5.8 build script with timestamped logs.
#>

[CmdletBinding()]
param(
    [string]$EngineRoot = 'I:\Spil\Epic Games\UE_5.8',
    [ValidateSet('Development','DebugGame')]
    [string]$Configuration = 'Development'
)

$ErrorActionPreference = 'Stop'
$ScriptVersion = '1.0.0'
$RepoRoot = Split-Path -Parent $PSScriptRoot
$Project = Join-Path $RepoRoot 'MariaGame.uproject'
$BuildBat = Join-Path $EngineRoot 'Engine\Build\BatchFiles\Build.bat'
$LogDir = Join-Path $PSScriptRoot 'BuildLogs'
$Timestamp = Get-Date -Format 'yyyyMMdd_HHmmss'
$LogFile = Join-Path $LogDir ('MariaGame_' + $Timestamp + '.log')

function Write-Checkpoint {
    param([string]$Name, [string]$Message)
    $Line = '[{0}] CP:{1} {2}' -f (Get-Date -Format 'HH:mm:ss'), $Name, $Message
    Write-Host $Line
    Add-Content -Path $LogFile -Value $Line
}

New-Item -ItemType Directory -Path $LogDir -Force | Out-Null
Set-Content -Path $LogFile -Value ('MariaGame build log - ' + (Get-Date -Format 'yyyy-MM-dd HH:mm:ss'))

Write-Checkpoint 'START' ('Build-MariaGame.ps1 v' + $ScriptVersion)
Write-Checkpoint 'ENGINE' $EngineRoot
Write-Checkpoint 'PROJECT' $Project

if (-not (Test-Path $BuildBat)) { throw ('Build.bat not found: ' + $BuildBat) }
if (-not (Test-Path $Project)) { throw ('Project not found: ' + $Project) }

Write-Checkpoint 'BUILD' ('MariaGameEditor Win64 ' + $Configuration)

$Arguments = @(
    'MariaGameEditor',
    'Win64',
    $Configuration,
    ('-Project=' + $Project),
    '-WaitMutex'
)

& $BuildBat @Arguments 2>&1 | Tee-Object -FilePath $LogFile -Append
$ExitCode = $LASTEXITCODE

if ($ExitCode -eq 0) {
    Write-Checkpoint 'DONE' 'Build succeeded.'
} else {
    Write-Checkpoint 'FAIL' ('Build failed with exit code ' + $ExitCode)
}

Write-Host ('Log: ' + $LogFile)
exit $ExitCode
