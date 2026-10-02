<#
.SYNOPSIS
    Safely updates MariaGame from GitHub.

.DESCRIPTION
    Refuses to pull when tracked local changes exist. Uses cmd.exe for Git
    fetch/pull output so normal Git progress is not surfaced as a PowerShell
    NativeCommandError.

.VERSION
    1.0.0

.CHANGELOG
    1.0.0 - Initial MariaGame updater.
#>

[CmdletBinding()]
param()

$ErrorActionPreference = 'Stop'
$ScriptVersion = '1.0.0'
$RepoRoot = Split-Path -Parent $PSScriptRoot

function Write-Checkpoint {
    param([string]$Name, [string]$Message)
    Write-Host ('[{0}] CP:{1} {2}' -f (Get-Date -Format 'HH:mm:ss'), $Name, $Message)
}

Write-Checkpoint 'START' ('Update-MariaGame.ps1 v' + $ScriptVersion)
Write-Checkpoint 'ROOT' $RepoRoot

if (-not (Test-Path (Join-Path $RepoRoot '.git'))) {
    throw ('Git repository not found at ' + $RepoRoot)
}

$Status = & git -C $RepoRoot status --porcelain 2>$null
if ($LASTEXITCODE -ne 0) { throw 'git status failed.' }

if ($Status) {
    Write-Host ''
    Write-Warning 'Local changes detected. Pull aborted to protect your work.'
    $Status | ForEach-Object { Write-Host ('  ' + $_) }
    Write-Host ''
    Write-Host 'Commit/stash the changes, or review them before updating.'
    exit 2
}

Write-Checkpoint 'FETCH' 'Fetching origin...'
$FetchCommand = 'git -C "{0}" fetch origin' -f $RepoRoot
& cmd.exe /d /c $FetchCommand
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Checkpoint 'PULL' 'Fast-forwarding current branch...'
$PullCommand = 'git -C "{0}" pull --ff-only' -f $RepoRoot
& cmd.exe /d /c $PullCommand
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Checkpoint 'STATUS' 'Repository status after update:'
& git -C $RepoRoot status --short --branch
Write-Checkpoint 'DONE' 'MariaGame is updated.'
