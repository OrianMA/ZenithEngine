@echo off
setlocal
REM One-click installer + builder for ZenithEngine
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0scripts\install_and_build.ps1" %*
echo.
echo Done. Press any key to close.
pause >nul
endlocal

