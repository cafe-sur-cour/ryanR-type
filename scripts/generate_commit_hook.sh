#!/bin/bash

# Script to generate the conventional commit hook

HOOK_DIR=".git/hooks"
HOOK_FILE="$HOOK_DIR/commit-msg"

mkdir -p "$HOOK_DIR"

cat > "$HOOK_FILE" << 'EOF'
#!/bin/bash

# Hook to enforce conventional commit messages
# Format: type(scope): description
# Types: feat, fix, docs, style, refactor, perf, test, chore

COMMIT_MSG_FILE=$1
COMMIT_MSG=$(cat $COMMIT_MSG_FILE)

# Regex for conventional commit
REGEX="^(feat|fix|docs|style|refactor|perf|test|chore)(\(.+\))?: .+"

if ! [[ $COMMIT_MSG =~ $REGEX ]]; then
    echo -e "\033[0;31mERROR: Commit message does not follow conventional commit format.\033[0m"
    echo -e "\033[0;33mExpected format: type(scope): description\033[0m"
    echo -e "\033[0;33mTypes: feat, fix, docs, style, refactor, perf, test, chore\033[0m"
    echo -e "\033[0;33mExample: feat: add new feature\033[0m"
    echo -e "\033[0;33mYour message: $COMMIT_MSG\033[0m"
    exit 1
fi

# Check if committing to main branch
CURRENT_BRANCH=$(git rev-parse --abbrev-ref HEAD)
if [ "$CURRENT_BRANCH" = "main" ]; then
    echo -e "\033[0;31mWARNING: You are committing directly to the main branch!\033[0m"
fi

exit 0
EOF

chmod +x "$HOOK_FILE"

echo "Conventional commit hook generated at $HOOK_FILE"
