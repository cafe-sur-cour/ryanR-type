@echo off
setlocal enabledelayedexpansion

REM Check if VCPKG_ROOT is set
if "%VCPKG_ROOT%"=="" (
    echo Error: VCPKG_ROOT environment variable is not set.
    exit /b 1
)

set build_tests=OFF
set target=

REM Parse command line arguments
:parse_args
if "%~1"=="" goto end_parse
if /i "%~1"=="--with-tests" (
    set build_tests=ON
    shift
    goto parse_args
)
if /i "%~1"=="--help" goto show_help
if /i "%~1"=="-h" goto show_help
set target=%~1
shift
goto parse_args

:show_help
echo Usage: %~nx0 [--with-tests] [^<target^>]
echo   --with-tests : Build with tests
echo   ^<target^>     : (Optional) The specific target to build (all, server, client, tests).
exit /b 0

:end_parse

REM Configure CMake with the appropriate preset
cmake --preset "release-windows" -DBUILD_TESTS=%build_tests%
if errorlevel 1 (
    echo CMake configuration failed.
    exit /b 1
)

REM Build the project
if "%target%"=="" (
    cmake --build --preset "release-windows"
    exit /b %errorlevel%
)

if /i "%target%"=="all" (
    cmake --build --preset "release-windows"
    exit /b %errorlevel%
)

cmake --build --preset "release-windows" --target %target%
exit /b %errorlevel%
