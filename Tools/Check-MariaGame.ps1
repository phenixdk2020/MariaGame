<#
.SYNOPSIS
    Checks MariaGame development prerequisites.

.VERSION
    1.0.0
#>

[CmdletBinding()]
param(
    [string]$EngineRoot = 'I:\Spil\Epic Games\UE_5.8'
)

$ErrorActionPreference = 'Continue'
$RepoRoot = Split-Path -Parent $PSScriptRoot
$Project = Join-Path $RepoRoot 'MariaGame.uproject'
$BuildBat = Join-Path $EngineRoot 'Engine\Build\BatchFiles\Build.bat'
$EditorExe = Join-Path $EngineRoot 'Engine\Binaries\Win64\UnrealEditor.exe'

function Test-ItemState {
    param([string]$Name, [bool]$Ok, [string]$Detail)
    $State = if ($Ok) { 'OK' } else { 'FAIL' }
    Write-Host ('[{0}] {1} - {2}' -f $State, $Name, $Detail)
}

$Git = Get-Command git -ErrorAction SilentlyContinue
Test-ItemState 'Git' ($null -ne $Git) ($(if ($Git) { $Git.Source } else { 'not found' }))

$GitLfsOk = $false
$GitLfsText = 'not found'
if ($Git) {
    $Lfs = & git lfs version 2>$null
    if ($LASTEXITCODE -eq 0) {
        $GitLfsOk = $true
        $GitLfsText = ($Lfs -join ' ')
    }
}
Test-ItemState 'Git LFS' $GitLfsOk $GitLfsText

Test-ItemState 'Project' (Test-Path $Project) $Project
Test-ItemState 'UE Build.bat' (Test-Path $BuildBat) $BuildBat
Test-ItemState 'UnrealEditor.exe' (Test-Path $EditorExe) $EditorExe

if (Test-Path (Join-Path $RepoRoot '.git')) {
    $Branch = & git -C $RepoRoot branch --show-current 2>$null
    Test-ItemState 'Git repository' ($LASTEXITCODE -eq 0) ('branch=' + $Branch)
    Write-Host ''
    Write-Host 'Working tree:'
    & git -C $RepoRoot status --short --branch
} else {
    Test-ItemState 'Git repository' $false $RepoRoot
}

Write-Host ''
Write-Host 'If Git LFS is missing, install/enable it before binary Unreal assets are added.'
