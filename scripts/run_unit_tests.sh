#!/bin/bash

# Script to run unit tests

./scripts/compile_project.sh --with-tests
cd build/unix/bin
ctest --verbose
