#!/bin/bash

# Define the log file
LOG_FILE="coding-style-cpp-reports.log"
> "$LOG_FILE"  # Clear the log file before writing

# Define the cpplint filter to only check specific errors
FILTERS="+whitespace/line_length,+whitespace/operators,+readability/braces,+whitespace/tab,+readability/casting,+build/include_order,+whitespace/empty_if_body"

NOFILTERS="-readability/namespace,-legal/copyright,-build/include_subdir,-whitespace/parens"
# Run cpplint on all .cpp and .hpp files, excluding tests/ and bonus/
find . -type f \( -name "*.cpp" \) ! -path "*/tests/*" ! -path "*/bonus/*" ! -path "*/temp/*" ! -path "*/build/*" | while read -r file; do
    cpplint --filter=$FILTERS --filter=$NOFILTERS "$file" 2>> "$LOG_FILE"
done

# Inform user of completion
echo "Coding style check completed. See $LOG_FILE for details."

