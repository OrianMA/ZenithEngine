# ZenithEngine Test Runner Script
# This script provides easy commands to run unit tests

param(
    [string]$Filter = "",
    [switch]$Verbose,
    [switch]$Color,
    [switch]$Build,
    [switch]$Clean,
    [switch]$Help
)

# Configuration
$ProjectRoot = Split-Path -Parent $PSScriptRoot
$BuildDir = Join-Path $ProjectRoot "build"
$TestExe = Join-Path $BuildDir "Release\ZenithEngineTests.exe"

function Show-Help {
    Write-Host "ZenithEngine Test Runner" -ForegroundColor Cyan
    Write-Host "========================" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Usage:" -ForegroundColor Yellow
    Write-Host "  .\run_tests.ps1 [options]" -ForegroundColor White
    Write-Host ""
    Write-Host "Options:" -ForegroundColor Yellow
    Write-Host "  -Filter <pattern>    Run tests matching pattern (e.g., 'SimpleTest.*')" -ForegroundColor White
    Write-Host "  -Verbose            Run tests with verbose output" -ForegroundColor White
    Write-Host "  -Color              Run tests with colored output" -ForegroundColor White
    Write-Host "  -Build              Build project before running tests" -ForegroundColor White
    Write-Host "  -Clean              Clean build directory before building" -ForegroundColor White
    Write-Host "  -Help               Show this help message" -ForegroundColor White
    Write-Host ""
    Write-Host "Examples:" -ForegroundColor Yellow
    Write-Host "  .\run_tests.ps1                           # Run all tests" -ForegroundColor White
    Write-Host "  .\run_tests.ps1 -Filter 'SimpleTest.*'    # Run SimpleTest suite" -ForegroundColor White
    Write-Host "  .\run_tests.ps1 -Verbose -Color          # Run with verbose and colored output" -ForegroundColor White
    Write-Host "  .\run_tests.ps1 -Build                   # Build and run tests" -ForegroundColor White
    Write-Host "  .\run_tests.ps1 -Clean -Build            # Clean build and run tests" -ForegroundColor White
}

function Test-BuildDirectory {
    if (-not (Test-Path $BuildDir)) {
        Write-Host "Build directory not found. Creating..." -ForegroundColor Yellow
        New-Item -ItemType Directory -Path $BuildDir -Force | Out-Null
        return $false
    }
    return $true
}

function Invoke-CMakeConfigure {
    Write-Host "Configuring project with CMake..." -ForegroundColor Cyan
    Push-Location $BuildDir
    try {
        cmake ..
        if ($LASTEXITCODE -ne 0) {
            Write-Host "CMake configuration failed!" -ForegroundColor Red
            exit 1
        }
    }
    finally {
        Pop-Location
    }
}

function Invoke-Build {
    Write-Host "Building project..." -ForegroundColor Cyan
    Push-Location $BuildDir
    try {
        cmake --build . --config Release
        if ($LASTEXITCODE -ne 0) {
            Write-Host "Build failed!" -ForegroundColor Red
            exit 1
        }
    }
    finally {
        Pop-Location
    }
}

function Invoke-Clean {
    Write-Host "Cleaning build directory..." -ForegroundColor Cyan
    if (Test-Path $BuildDir) {
        Remove-Item -Path $BuildDir -Recurse -Force
    }
}

function Invoke-Tests {
    $args = @()
    
    if ($Filter) {
        $args += "--gtest_filter=$Filter"
    }
    
    if ($Verbose) {
        $args += "--gtest_verbose"
    }
    
    if ($Color) {
        $args += "--gtest_color=yes"
    }
    
    $args += "--gtest_print_time=1"
    
    Write-Host "Running tests..." -ForegroundColor Cyan
    Write-Host "Command: $TestExe $($args -join ' ')" -ForegroundColor Gray
    
    Push-Location $BuildDir
    try {
        & $TestExe @args
        $exitCode = $LASTEXITCODE
        
        if ($exitCode -eq 0) {
            Write-Host "`n✅ All tests passed!" -ForegroundColor Green
        } else {
            Write-Host "`n❌ Some tests failed!" -ForegroundColor Red
        }
        
        return $exitCode
    }
    finally {
        Pop-Location
    }
}

# Main execution
if ($Help) {
    Show-Help
    exit 0
}

# Check if test executable exists
if (-not (Test-Path $TestExe)) {
    Write-Host "Test executable not found. Building project..." -ForegroundColor Yellow
    $Build = $true
}

# Clean if requested
if ($Clean) {
    Invoke-Clean
}

# Build if requested or needed
if ($Build -or -not (Test-Path $TestExe)) {
    Test-BuildDirectory
    Invoke-CMakeConfigure
    Invoke-Build
}

# Run tests
$exitCode = Invoke-Tests
exit $exitCode
