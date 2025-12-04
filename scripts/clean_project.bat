@echo off
setlocal enabledelayedexpansion

if "%~1"=="--help" goto show_help
if "%~1"=="-h" goto show_help
goto clean

:show_help
echo Usage: %~nx0
echo   Cleans the build directory for the release-windows preset.
exit /b 0

:clean
echo Cleaning build directory...
if exist "build\windows" rmdir /s /q "build\windows"

echo Removing shared libraries...
if exist "libraries\*.dll" del /q "libraries\*.dll"
if exist "libraries\*.pdb" del /q "libraries\*.pdb"

echo Removing executables and related files from root...
if exist "r-type_server.exe" del /q "r-type_server.exe"
if exist "r-type_client.exe" del /q "r-type_client.exe"
if exist "*.dll" del /q "*.dll"
if exist "*.pdb" del /q "*.pdb"

echo Clean completed.
exit /b 0
