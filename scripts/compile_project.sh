#!/bin/bash

if [ -z "$VCPKG_ROOT" ]; then
    echo "Error: VCPKG_ROOT environment variable is not set."
    exit 1
fi

build_tests=false
build_debug=false
test_coverage=false
target=""

while [[ $# -gt 0 ]]; do
    case $1 in
        --with-tests)
            build_tests=true
            shift
            ;;
        --with-coverage)
            test_coverage=true
            shift
            ;;
        --debug)
            build_debug=true
            shift
            ;;
        --help|-h)
            echo "Usage: $0 [--with-tests] [--debug] [<target>]"
            echo "  --with-tests : Build with tests"
            echo "  --debug      : Build in debug mode"
            echo "  <target>     : (Optional) The specific target to build (all, server, client, tests)."
            echo ""
            echo "When specifying a target, all dependencies (common + libs) will be built automatically."
            exit 0
            ;;
        *)
            target=$1
            shift
            ;;
    esac
done

build_client_option="ON"
build_server_option="ON"
build_tests_option="OFF"
build_coverage_option="OFF"
preset="release-unix"

if [ "$build_tests" = true ]; then
    build_tests_option="ON"
    if [ "$test_coverage" = true ]; then
        build_coverage_option="ON"
    fi
fi

# No need to change preset; we'll override CMAKE_BUILD_TYPE below

case "$target" in
    "server"|"r-type_server")
        echo "Building server with all dependencies (common + libs)..."
        build_client_option="OFF"
        build_server_option="ON"
        ;;
    "client"|"r-type_client")
        echo "Building client with all dependencies (common + libs + multimedia)..."
        build_client_option="ON"
        build_server_option="OFF"
        ;;
    "tests")
        echo "Building tests with all dependencies..."
        build_tests_option="ON"
        # Tests may need both client and server libs, keep both ON
        build_client_option="ON"
        build_server_option="ON"
        ;;
    "all"|"")
        echo "Building all targets..."
        build_client_option="ON"
        build_server_option="ON"
        ;;
    *)
        echo "Unknown target: $target"
        echo "Valid targets: all, server, client, tests"
        exit 1
        ;;
esac

# Build the cmake command with conditional CMAKE_BUILD_TYPE
cmake_cmd="cmake --preset \"$preset\" -DBUILD_TESTS=\"$build_tests_option\" -DBUILD_CLIENT=\"$build_client_option\" -DBUILD_SERVER=\"$build_server_option\" -DBUILD_COVERAGE=\"$build_coverage_option\""
if [ "$build_debug" = true ]; then
    cmake_cmd="$cmake_cmd -DCMAKE_BUILD_TYPE=Debug"
fi

eval "$cmake_cmd"

if [ $? -ne 0 ]; then
    echo "CMake configuration failed."
    exit 1
fi

cmake --build --preset "$preset" -j8

exit $?
