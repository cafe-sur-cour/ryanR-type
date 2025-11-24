#!/bin/bash

# Script to run unit tests

# Check if build directory exists
if [ ! -d "build/unix/bin" ]; then
    echo "Build directory not found. Please run compile_project.sh first."
    exit 1
fi

# Change to the build directory
cd build/unix/bin

# Check if CTestTestfile.cmake exists
if [ ! -f "CTestTestfile.cmake" ]; then
    echo "Test configuration not found. Please re-run compile_project.sh to include tests."
    exit 1
fi

# Run the tests using ctest with verbose output
ctest --verbose