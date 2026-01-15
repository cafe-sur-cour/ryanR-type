#!/bin/bash

# Script to run unit tests

./scripts/compile_project.sh --with-tests $@

if [ $? -ne 0 ]; then
    echo "Compilation failed. Cannot run unit tests."
    exit 1
fi

cd build/unix/bin

ctest --verbose --no-tests=error

if [ $? -ne 0 ]; then
    echo "Some unit tests failed."
    exit 1
fi
