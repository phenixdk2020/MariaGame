<#
.SYNOPSIS
    Updates MariaGame and then builds MariaGameEditor.

.VERSION
    1.0.0
#>

[CmdletBinding()]
param(
    [string]$EngineRoot = 'I:\Spil\Epic Games\UE_5.8'
)

$ErrorActionPreference = 'Stop'

& (Join-Path $PSScriptRoot 'Update-MariaGame.ps1')
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

& (Join-Path $PSScriptRoot 'Build-MariaGame.ps1') -EngineRoot $EngineRoot
exit $LASTEXITCODE
