#!/bin/bash

if [ "$1" == "--help" ] || [ "$1" == "-h" ]; then
    echo "Usage: $0"
    echo "  Cleans the build directory for the release-unix preset."
    exit 0
fi

echo "Cleaning build directory..."
rm -rf build/unix/bin

echo "Clean completed."
