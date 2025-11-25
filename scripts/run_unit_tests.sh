#!/bin/bash

# Script to run unit tests

# Check if build directory exists
if [ ! -d "build/unix/bin" ]; then
    echo "Build directory not found. Building tests..."
    ./scripts/compile_project.sh --with-tests
fi

# Change to the build directory
cd build/unix/bin

# Check if CTestTestfile.cmake exists
if [ ! -f "CTestTestfile.cmake" ]; then
    echo "Test configuration not found. Building tests..."
    cd ../../..
    ./scripts/compile_project.sh --with-tests
    cd build/unix/bin
fi

# Check if tests are available
if ctest --show-only | grep -q "Total Tests: 0"; then
    echo "No tests found in current build. Rebuilding with tests..."
    cd ../../..
    ./scripts/compile_project.sh --with-tests
    cd build/unix/bin
fi

# Run the tests using ctest with verbose output
ctest --verbose