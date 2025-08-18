@echo off
REM ZenithEngine Test Runner Script (Windows Batch)
REM This script provides easy commands to run unit tests

setlocal enabledelayedexpansion

REM Configuration
set "PROJECT_ROOT=%~dp0.."
set "BUILD_DIR=%PROJECT_ROOT%\build"
set "TEST_EXE=%BUILD_DIR%\Release\ZenithEngineTests.exe"

REM Parse command line arguments
set "FILTER="
set "VERBOSE="
set "COLOR="
set "BUILD="
set "CLEAN="
set "HELP="

:parse_args
if "%1"=="" goto :main
if /i "%1"=="-Filter" (
    set "FILTER=%2"
    shift
    shift
    goto :parse_args
)
if /i "%1"=="-Verbose" (
    set "VERBOSE=--gtest_verbose"
    shift
    goto :parse_args
)
if /i "%1"=="-Color" (
    set "COLOR=--gtest_color=yes"
    shift
    goto :parse_args
)
if /i "%1"=="-Build" (
    set "BUILD=1"
    shift
    goto :parse_args
)
if /i "%1"=="-Clean" (
    set "CLEAN=1"
    shift
    goto :parse_args
)
if /i "%1"=="-Help" (
    set "HELP=1"
    shift
    goto :parse_args
)
shift
goto :parse_args

:show_help
echo ZenithEngine Test Runner
echo ======================
echo.
echo Usage:
echo   run_tests.bat [options]
echo.
echo Options:
echo   -Filter pattern    Run tests matching pattern (e.g., SimpleTest.*)
echo   -Verbose          Run tests with verbose output
echo   -Color            Run tests with colored output
echo   -Build            Build project before running tests
echo   -Clean            Clean build directory before building
echo   -Help             Show this help message
echo.
echo Examples:
echo   run_tests.bat                           # Run all tests
echo   run_tests.bat -Filter SimpleTest.*      # Run SimpleTest suite
echo   run_tests.bat -Verbose -Color          # Run with verbose and colored output
echo   run_tests.bat -Build                   # Build and run tests
echo   run_tests.bat -Clean -Build            # Clean build and run tests
goto :end

:test_build_directory
if not exist "%BUILD_DIR%" (
    echo Build directory not found. Creating...
    mkdir "%BUILD_DIR%"
    exit /b 0
)
exit /b 1

:invoke_cmake_configure
echo Configuring project with CMake...
cd /d "%BUILD_DIR%"
cmake ..
if errorlevel 1 (
    echo CMake configuration failed!
    exit /b 1
)
cd /d "%~dp0"
exit /b 0

:invoke_build
echo Building project...
cd /d "%BUILD_DIR%"
cmake --build . --config Release
if errorlevel 1 (
    echo Build failed!
    exit /b 1
)
cd /d "%~dp0"
exit /b 0

:invoke_clean
echo Cleaning build directory...
if exist "%BUILD_DIR%" (
    rmdir /s /q "%BUILD_DIR%"
)
exit /b 0

:invoke_tests
echo Running tests...

set "ARGS=--gtest_print_time=1"
if defined FILTER set "ARGS=%ARGS% --gtest_filter=%FILTER%"
if defined VERBOSE set "ARGS=%ARGS% %VERBOSE%"
if defined COLOR set "ARGS=%ARGS% %COLOR%"

echo Command: %TEST_EXE% %ARGS%

cd /d "%BUILD_DIR%"
%TEST_EXE% %ARGS%
set "EXIT_CODE=%ERRORLEVEL%"
cd /d "%~dp0"

if %EXIT_CODE% equ 0 (
    echo.
    echo ✅ All tests passed!
) else (
    echo.
    echo ❌ Some tests failed!
)

exit /b %EXIT_CODE%

:main
if defined HELP goto :show_help

REM Check if test executable exists
if not exist "%TEST_EXE%" (
    echo Test executable not found. Building project...
    set "BUILD=1"
)

REM Clean if requested
if defined CLEAN call :invoke_clean

REM Build if requested or needed
if defined BUILD (
    call :test_build_directory
    call :invoke_cmake_configure
    call :invoke_build
)

REM Run tests
call :invoke_tests

:end
endlocal
