param(
    [ValidateSet("Debug", "Release")]
    [string]$Config = "Release",
    [ValidateSet("x64", "Win32", "arm64")]
    [string]$Arch = "x64"
)

$ErrorActionPreference = "Stop"

function Write-Section($msg) { Write-Host "`n=== $msg ===" -ForegroundColor Cyan }
function Write-Step($msg) { Write-Host "[+] $msg" -ForegroundColor Green }
function Write-Warn($msg) { Write-Warning $msg }
function Test-Admin {
    $currentIdentity = [Security.Principal.WindowsIdentity]::GetCurrent()
    $principal = New-Object Security.Principal.WindowsPrincipal($currentIdentity)
    return $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
}

if (-not (Test-Admin)) {
    Write-Host "Elevating to Administrator..." -ForegroundColor Yellow
    $args = @()
    if ($PSBoundParameters.ContainsKey('Config')) { $args += "-Config", $Config }
    if ($PSBoundParameters.ContainsKey('Arch')) { $args += "-Arch", $Arch }
    Start-Process -FilePath "powershell" -Verb RunAs -ArgumentList @('-NoProfile','-ExecutionPolicy','Bypass','-File',"`"$PSCommandPath`"" ) + $args | Out-Null
    exit 0
}

$RepoRoot = Split-Path -Parent $PSScriptRoot
$BuildDir = Join-Path $RepoRoot "build"
$VcpkgDir  = Join-Path $RepoRoot "vcpkg"
$Toolchain = Join-Path $VcpkgDir  "scripts\buildsystems\vcpkg.cmake"

function Test-Command($name) { return [bool](Get-Command $name -ErrorAction SilentlyContinue) }

function Get-PackageManager {
    if (Test-Command winget) { return 'winget' }
    if (Test-Command choco)  { return 'choco' }
    return $null
}

function Install-PackageWinget($id, $override = $null) {
    $args = @('install','--id', $id,'-e','--accept-package-agreements','--accept-source-agreements','--source','winget')
    if ($override) { $args += @('--override', $override) }
    & winget @args
}

function Install-PackageChoco($name, $params = $null) {
    $args = @('install', $name, '-y')
    if ($params) { $args += @('--package-parameters', $params) }
    & choco @args
}

function Ensure-Tool($displayName, $test, $wingetId, $chocoName, $wingetOverride = $null, $chocoParams = $null) {
    if (& $test) { Write-Step "$displayName trouvé."; return }
    $pm = Get-PackageManager
    if (-not $pm) { throw "Aucun gestionnaire de paquets détecté (winget/choco). Installez $displayName manuellement puis relancez." }

    Write-Section "Installation de $displayName via $pm"
    if ($pm -eq 'winget') { Install-PackageWinget -id $wingetId -override $wingetOverride }
    else { Install-PackageChoco -name $chocoName -params $chocoParams }

    if (-not (& $test)) { throw "$displayName n'a pas pu être installé automatiquement. Veuillez l'installer manuellement puis relancer." }
    Write-Step "$displayName installé."
}

function Test-VisualStudioInstalled {
    try {
        $vswhere = "$Env:ProgramFiles(x86)\Microsoft Visual Studio\Installer\vswhere.exe"
        if (Test-Path $vswhere) {
            $path = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Workload.VCTools -property installationPath 2>$null
            return [bool]$path
        }
        return $false
    } catch { return $false }
}

Write-Section "Vérification des prérequis"

Ensure-Tool -displayName 'Git'    -test { Test-Command git }    -wingetId 'Git.Git' -chocoName 'git'
Ensure-Tool -displayName 'CMake'  -test { Test-Command cmake }  -wingetId 'Kitware.CMake' -chocoName 'cmake'

if (-not (Test-VisualStudioInstalled)) {
    Write-Section "Installation des Visual Studio Build Tools (C++ Toolset)"
    $pm = Get-PackageManager
    if (-not $pm) { throw "Aucun gestionnaire de paquets détecté pour installer Visual Studio Build Tools." }
    if ($pm -eq 'winget') {
        # Installe le toolset C++ + composants recommandés
        Install-PackageWinget -id 'Microsoft.VisualStudio.2022.BuildTools' -override '--add Microsoft.VisualStudio.Workload.VCTools --includeRecommended --passive --norestart'
    } else {
        Install-PackageChoco -name 'visualstudio2022buildtools' -params '"--add Microsoft.VisualStudio.Workload.VCTools --includeRecommended --passive --norestart"'
    }

    if (-not (Test-VisualStudioInstalled)) {
        Write-Warn "Visual Studio Build Tools non détecté après installation. Le build peut échouer si le toolset C++ n'est pas disponible."
    } else {
        Write-Step "Visual Studio Build Tools prêts."
    }
}

Write-Section "Configuration de vcpkg"

if (-not (Test-Path $VcpkgDir)) {
    Write-Step "Init du submodule vcpkg"
    Push-Location $RepoRoot
    git submodule update --init --recursive vcpkg
    Pop-Location
}

$VcpkgExe = Join-Path $VcpkgDir 'vcpkg.exe'
if (-not (Test-Path $VcpkgExe)) {
    Write-Step "Bootstrap de vcpkg (compilation de l'outil)"
    & "$VcpkgDir\bootstrap-vcpkg.bat" -disableMetrics
}
if (-not (Test-Path $VcpkgExe)) { throw "vcpkg.exe introuvable après bootstrap." }

$env:VCPKG_ROOT = $VcpkgDir
$env:VCPKG_FEATURE_FLAGS = 'manifests,registries'

Write-Section "Préinstallation des dépendances (manifest)"
try {
    & $VcpkgExe install --triplet "$Arch-windows" 2>$null | Out-Null
} catch {
    Write-Warn "Préinstallation vcpkg optionnelle a échoué; CMake téléchargera à la configuration. ($_ )"
}

Write-Section "Configuration et build CMake"
if (-not (Test-Path $BuildDir)) { New-Item -ItemType Directory -Path $BuildDir | Out-Null }

$generator = 'Visual Studio 17 2022'
$toolchainArg = "-DCMAKE_TOOLCHAIN_FILE=$Toolchain"

Write-Step "Configure ($generator | $Arch | $Config)"
cmake -S $RepoRoot -B $BuildDir -G "$generator" -A $Arch $toolchainArg

Write-Step "Build ($Config)"
cmake --build $BuildDir --config $Config

Write-Section "Terminé"
Write-Host "Binaire construit dans: $BuildDir" -ForegroundColor Green

