#!/bin/bash

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}Starting code coverage generation...${NC}"

# Check if gcovr is installed
if ! command -v gcovr &> /dev/null; then
    echo -e "${RED}Error: gcovr is not installed${NC}"
    echo "Install it with: pip install gcovr"
    exit 1
fi

# Get the script directory and project root
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/build/unix"

# Clean old coverage data
echo -e "${YELLOW}Cleaning old coverage data...${NC}"
find . -name "*.gcda" -delete
find . -name "*.gcov" -delete

# Build the tests
echo -e "${YELLOW}Building tests with coverage...${NC}"
$PROJECT_ROOT/scripts/compile_project.sh --with-tests --with-coverage

cd "$BUILD_DIR/bin"

# Run tests
echo -e "${YELLOW}Running tests...${NC}"
ctest --output-on-failure

# Generate coverage report
echo -e "${YELLOW}Generating coverage report...${NC}"

# HTML report
gcovr -r "$PROJECT_ROOT" \
    --exclude "$PROJECT_ROOT/tests" \
    --exclude "$PROJECT_ROOT/build" \
    --exclude "$PROJECT_ROOT/libs" \
    --html --html-details \
    -o "$BUILD_DIR/coverage.html"

# Terminal summary
echo ""
echo -e "${GREEN}Coverage Summary:${NC}"
gcovr -r "$PROJECT_ROOT" \
    --exclude "$PROJECT_ROOT/tests" \
    --exclude "$PROJECT_ROOT/build" \
    --exclude "$PROJECT_ROOT/libs"

echo ""
echo -e "${GREEN}Coverage report generated successfully!${NC}"
echo -e "HTML report: ${YELLOW}$BUILD_DIR/coverage.html${NC}"
echo ""
echo "To view the HTML report, open it in your browser"
