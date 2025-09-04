param(
    [string]$Config = "Debug",
    [string]$Arch = "x64",
    [string]$Generator = "Visual Studio 17 2022"
)

$ErrorActionPreference = "Stop"

$RepoRoot = Split-Path -Parent $PSScriptRoot
$BuildDir = Join-Path $RepoRoot "build"
$Toolchain = Join-Path $RepoRoot "vcpkg\scripts\buildsystems\vcpkg.cmake"

if (Test-Path $BuildDir) {
    Write-Host "Cleaning build directory (best-effort): $BuildDir"
    try {
        Remove-Item -Recurse -Force $BuildDir
    } catch {
        Write-Warning "Build dir in use; proceeding with in-place reconfigure. ($_ )"
    }
}

$env:VCPKG_FEATURE_FLAGS = "manifests"

Write-Host "Configuring CMake..."
cmake -S $RepoRoot -B $BuildDir -G "$Generator" -A $Arch -DCMAKE_TOOLCHAIN_FILE=$Toolchain

Write-Host "Building ($Config)..."
cmake --build $BuildDir --config $Config

Write-Host "Done."
