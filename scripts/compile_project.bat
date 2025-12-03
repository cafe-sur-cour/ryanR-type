@echo off
setlocal enabledelayedexpansion

REM Check if VCPKG_ROOT is set
if "%VCPKG_ROOT%"=="" (
    echo Error: VCPKG_ROOT environment variable is not set.
    exit /b 1
)

set build_tests=OFF
set build_client=ON
set build_server=ON
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
echo.
echo When specifying a target, all dependencies (common + libs) will be built automatically.
exit /b 0

:end_parse

REM Determine BUILD_CLIENT and BUILD_TESTS based on target
if /i "%target%"=="server" goto set_server_options
if /i "%target%"=="r-type_server" goto set_server_options
if /i "%target%"=="client" goto set_client_options
if /i "%target%"=="r-type_client" goto set_client_options
if /i "%target%"=="tests" goto set_tests_options
if /i "%target%"=="all" goto set_all_options
if not "%target%"=="" goto unknown_target
goto set_all_options

:set_server_options
echo Building server with all dependencies (common + libs)...
set build_client=OFF
set build_server=ON
goto configure_cmake

:set_client_options
echo Building client with all dependencies (common + libs + multimedia)...
set build_client=ON
set build_server=OFF
goto configure_cmake

:set_tests_options
echo Building tests with all dependencies...
set build_tests=ON
set build_client=ON
set build_server=ON
goto configure_cmake

:set_all_options
echo Building all targets...
set build_client=ON
set build_server=ON
goto configure_cmake

:unknown_target
echo Unknown target: %target%
echo Valid targets: all, server, client, tests
exit /b 1

:configure_cmake
REM Configure CMake with the appropriate preset
cmake --preset "release-windows" -DBUILD_TESTS=%build_tests% -DBUILD_CLIENT=%build_client% -DBUILD_SERVER=%build_server%
if errorlevel 1 (
    echo CMake configuration failed.
    exit /b 1
)

REM Build the project - CMake will handle dependencies automatically based on BUILD_CLIENT/BUILD_SERVER options
cmake --build --preset "release-windows" --config Release -j4
if errorlevel 1 (
    echo Build failed.
    exit /b 1
)
exit /b 0
