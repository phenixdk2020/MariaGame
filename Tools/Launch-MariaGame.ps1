<#
.SYNOPSIS
    Launches MariaGame in Unreal Editor 5.8.

.VERSION
    1.0.0
#>

[CmdletBinding()]
param(
    [string]$EngineRoot = 'I:\Spil\Epic Games\UE_5.8'
)

$ErrorActionPreference = 'Stop'
$RepoRoot = Split-Path -Parent $PSScriptRoot
$Project = Join-Path $RepoRoot 'MariaGame.uproject'
$EditorExe = Join-Path $EngineRoot 'Engine\Binaries\Win64\UnrealEditor.exe'

if (-not (Test-Path $EditorExe)) { throw ('UnrealEditor.exe not found: ' + $EditorExe) }
if (-not (Test-Path $Project)) { throw ('Project not found: ' + $Project) }

Write-Host ('Launching: ' + $Project)
Start-Process -FilePath $EditorExe -ArgumentList @($Project)
