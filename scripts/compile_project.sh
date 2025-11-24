#!/bin/bash

if [ -z "$VCPKG_ROOT" ]; then
    echo "Error: VCPKG_ROOT environment variable is not set."
    exit 1
fi

if [ "$1" == "--help" ] || [ "$1" == "-h" ]; then
    echo "Usage: $0 [<target>]"
    echo "  <target> : (Optional) The specific target to build (all, server, client, tests)."
    exit 0
fi

target=$1

cmake --preset "release-unix"

if [ -z "$target" || "$target" == "all" ]; then
    cmake --build --preset "release-unix"
    exit $?
fi

cmake --build --preset "release-unix" --target "$target"
