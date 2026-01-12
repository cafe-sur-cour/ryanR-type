#!/bin/bash

# Script to build the frontend
# This script builds the Vue.js frontend and ensures the dist/ folder is ready for the server

set -e  # Exit on any error

FRONTEND_DIR="$(dirname "$0")/../frontend"
DIST_DIR="dist"

echo "Building frontend..."

# Check if npm is installed
if ! command -v npm &> /dev/null; then
    echo "Error: npm is not installed. Please install Node.js and npm."
    exit 1
fi

# Go to frontend directory
cd "$FRONTEND_DIR"

# Install dependencies if node_modules doesn't exist
if [ ! -d "node_modules" ]; then
    echo "Installing npm dependencies..."
    npm install
fi

# Build the frontend
echo "Building Vue.js application..."
npm run build

# Wait a bit for build to complete
sleep 1

# Check if dist directory was created
if [ ! -d "$DIST_DIR" ]; then
    echo "Error: Build failed - dist directory not found"
    exit 1
fi

echo "Frontend build completed successfully!"
echo "Dist files are in: $DIST_DIR"
echo "The server will serve these files on port 8080"