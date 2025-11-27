#!/bin/bash

if [ -z "$VCPKG_ROOT" ]; then
    echo "Error: VCPKG_ROOT environment variable is not set."
    exit 1
fi

build_tests=false

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
            exit 0
            ;;
        *)
            target=$1
            shift
            ;;
    esac
done

if [ "$build_tests" = true ]; then
    cmake --preset "release-unix" -DBUILD_TESTS=ON
else
    cmake --preset "release-unix" -DBUILD_TESTS=OFF
fi

if [ $? -ne 0 ]; then
    echo "CMake configuration failed."
    exit 1
fi

if [ -z "$target" ] || [ "$target" == "all" ]; then
    cmake --build --preset "release-unix"
    exit $?
fi

if [ "$target" == "server" ] || [ "$target" == "r-type_server" ]; then
    echo "Building server only (BUILD_CLIENT=OFF)..."
    if [ "$build_tests" = true ]; then
        cmake --preset "release-unix" -DBUILD_TESTS=ON -DBUILD_CLIENT=OFF
    else
        cmake --preset "release-unix" -DBUILD_TESTS=OFF -DBUILD_CLIENT=OFF
    fi
    if [ $? -ne 0 ]; then
        echo "CMake reconfiguration failed."
        exit 1
    fi
fi

cmake --build --preset "release-unix" --target "$target"
exit $?
