#!/bin/bash

if [ -z "$VCPKG_ROOT" ]; then
    echo "Error: VCPKG_ROOT environment variable is not set."
    exit 1
fi

build_tests=false
target=""

while [[ $# -gt 0 ]]; do
    case $1 in
        --with-tests)
            build_tests=true
            shift
            ;;
        --help|-h)
            echo "Usage: $0 [--with-tests] [<target>]"
            echo "  --with-tests : Build with tests"
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

if [ "$build_tests" = true ]; then
    build_tests_option="ON"
fi

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

cmake --preset "release-unix" -DBUILD_TESTS="$build_tests_option" -DBUILD_CLIENT="$build_client_option" -DBUILD_SERVER="$build_server_option"

if [ $? -ne 0 ]; then
    echo "CMake configuration failed."
    exit 1
fi

cmake --build --preset "release-unix"

exit $?
