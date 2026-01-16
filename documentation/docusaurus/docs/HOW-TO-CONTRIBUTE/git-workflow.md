---
sidebar_position: 3
---

# Git Workflow

Complete guide for contributing code using Git and GitHub.

## Repository Structure

### Main Branches

- **`main`**: Production-ready code, always stable
- **`develop`**: Integration branch for features (if used)

### Branch Naming Convention

```
<type>/<short-description>
```

**Types:**
- `feature/`: New feature development
- `fix/`: Bug fixes
- `docs/`: Documentation changes
- `refactor/`: Code refactoring
- `test/`: Test additions or modifications

**Examples:**
```bash
feature/lobby-system
fix/network-disconnect-bug
docs/update-readme
refactor/ecs-registry
test/add-collision-tests
```

## Contribution Workflow

### 1. Fork and Clone

**Fork the repository** on GitHub, then clone it:

```bash
# Clone your fork
git clone https://github.com/YOUR_USERNAME/R-Type.git
cd R-Type

# Add upstream remote
git remote add upstream https://github.com/AlbanRSS/doc-rtype.git
```

### 2. Create a Branch

```bash
# Update your local main
git checkout main
git pull upstream main

# Create and switch to new branch
git checkout -b feature/my-new-feature
```

### 3. Make Changes

Edit files, add features, fix bugs...

```bash
# Check status
git status

# See changes
git diff
```

### 4. Commit Changes

**Stage changes:**
```bash
# Stage specific files
git add file1.cpp file2.hpp

# Stage all changes
git add .

# Stage interactively
git add -p
```

**Commit with conventional message:**
```bash
git commit -m "feat(client): add player name input field

- Add TextBox widget to lobby screen
- Validate player name length (3-20 chars)
- Update SettingsManager to store name

Closes #42"
```

**Commit message format:**
```
<type>(<scope>): <subject>

<body>

<footer>
```

### 5. Push Branch

```bash
# First push
git push -u origin feature/my-new-feature

# Subsequent pushes
git push
```

### 6. Create Pull Request

1. Go to your fork on GitHub
2. Click **"Compare & pull request"**
3. Fill in PR template:
   - **Title**: Brief description
   - **Description**: Detailed explanation
   - **Related Issues**: Link issues (#42)
   - **Screenshots**: If applicable
4. Request review
5. Click **"Create pull request"**

### 7. Address Review Feedback

**Make changes based on review:**
```bash
# Make requested changes
git add modified_files
git commit -m "fix(client): address review feedback"
git push
```

**Amend last commit if needed:**
```bash
git add .
git commit --amend --no-edit
git push --force
```

### 8. Merge

Once approved, a maintainer will merge your PR.

## Keeping Your Fork Updated

### Sync with Upstream

```bash
# Fetch upstream changes
git fetch upstream

# Switch to main
git checkout main

# Merge upstream changes
git merge upstream/main

# Push to your fork
git push origin main
```

### Rebase Feature Branch

Keep your feature branch up-to-date:

```bash
# Switch to your feature branch
git checkout feature/my-feature

# Rebase on latest main
git rebase main

# If conflicts, resolve them and continue
git add resolved_files
git rebase --continue

# Force push (rebase rewrites history)
git push --force
```

## Common Git Commands

### Viewing History

```bash
# Commit log
git log

# Compact log
git log --oneline

# Graphical log
git log --graph --oneline --all

# File history
git log --follow file.cpp

# Changes in commits
git show <commit-hash>
```

### Undoing Changes

**Discard local changes (not staged):**
```bash
git restore file.cpp

# All files
git restore .
```

**Unstage changes:**
```bash
git restore --staged file.cpp
```

**Undo last commit (keep changes):**
```bash
git reset --soft HEAD~1
```

**Undo last commit (discard changes):**
```bash
git reset --hard HEAD~1
```

**Revert a pushed commit:**
```bash
git revert <commit-hash>
git push
```

### Stashing Changes

Save work in progress without committing:

```bash
# Stash changes
git stash

# Stash with message
git stash save "WIP: adding feature X"

# List stashes
git stash list

# Apply most recent stash
git stash apply

# Apply and remove stash
git stash pop

# Apply specific stash
git stash apply stash@{2}

# Drop stash
git stash drop stash@{0}
```

### Resolving Conflicts

**When conflicts occur during merge/rebase:**

```bash
# See conflicting files
git status

# Edit files to resolve conflicts
# Look for markers: <<<<<<<, =======, >>>>>>>

# After resolving
git add resolved_files

# Continue merge
git merge --continue

# Or continue rebase
git rebase --continue

# Abort if needed
git merge --abort
git rebase --abort
```

## Branch Management

### Listing Branches

```bash
# Local branches
git branch

# Remote branches
git branch -r

# All branches
git branch -a
```

### Switching Branches

```bash
# Switch to existing branch
git checkout branch-name

# Create and switch to new branch
git checkout -b new-branch

# Modern alternative (Git 2.23+)
git switch branch-name
git switch -c new-branch
```

### Deleting Branches

```bash
# Delete local branch (safe)
git branch -d feature/old-feature

# Force delete local branch
git branch -D feature/old-feature

# Delete remote branch
git push origin --delete feature/old-feature
```

## Pull Request Best Practices

### PR Checklist

Before creating a PR:

- [ ] Code follows [coding conventions](./coding-conventions.md)
- [ ] All tests pass locally
- [ ] New tests added for new features
- [ ] Documentation updated
- [ ] Commit messages follow conventions
- [ ] No merge conflicts with main
- [ ] PR is focused (one feature/fix per PR)

### PR Template

```markdown
## Description

Brief description of changes

## Related Issues

Closes #42
Relates to #38

## Type of Change

- [ ] Bug fix
- [ ] New feature
- [ ] Documentation update
- [ ] Refactoring
- [ ] Performance improvement

## Testing

Describe testing performed:
- Manual testing steps
- Automated tests added
- Edge cases considered

## Screenshots (if applicable)

Add screenshots for UI changes

## Checklist

- [ ] Code follows project style
- [ ] Tests pass
- [ ] Documentation updated
- [ ] No breaking changes (or documented)
```

### PR Review Process

1. **Automated Checks**: CI/CD runs tests
2. **Code Review**: Maintainer reviews code
3. **Feedback**: Address comments
4. **Approval**: PR approved by maintainer
5. **Merge**: Maintainer merges PR
6. **Branch Cleanup**: Delete feature branch

## Commit Message Guidelines

### Format

```
<type>(<scope>): <subject>

<body>

<footer>
```

### Types

| Type | Description | Example |
|------|-------------|---------|
| `feat` | New feature | `feat(client): add chat system` |
| `fix` | Bug fix | `fix(server): resolve disconnect crash` |
| `docs` | Documentation | `docs(readme): update installation steps` |
| `style` | Code style changes | `style(client): format according to guidelines` |
| `refactor` | Code refactoring | `refactor(ecs): optimize component storage` |
| `test` | Test additions | `test(network): add packet parsing tests` |
| `perf` | Performance | `perf(render): optimize sprite batching` |
| `chore` | Maintenance | `chore(deps): update SFML to 2.6` |

### Scope

Indicates the affected module:
- `client`: Client code
- `server`: Server code
- `ecs`: ECS framework
- `network`: Network code
- `ui`: User interface
- `docs`: Documentation
- `build`: Build system

### Subject

- Use imperative mood: "add" not "added"
- Don't capitalize first letter
- No period at the end
- Maximum 50 characters

### Body

- Explain **what** and **why**, not how
- Wrap at 72 characters
- Separate from subject with blank line

### Footer

- Reference issues: `Closes #42`
- Breaking changes: `BREAKING CHANGE: ...`

### Examples

**Simple commit:**
```
feat(client): add lobby chat system
```

**Detailed commit:**
```
feat(client): add lobby chat system

Implement text chat in lobby for player communication.
Includes:
- ChatWidget component
- Message history (max 100 messages)
- Auto-scroll to latest message
- Player name prefixing

Closes #42
```

**Bug fix:**
```
fix(server): prevent crash on client disconnect

Server would crash when client disconnected during gameplay.
Added null check before accessing client data.

Fixes #87
```

**Breaking change:**
```
refactor(ecs)!: change component API to use shared_ptr

BREAKING CHANGE: Components now require shared_ptr.
Update all component additions:
- Old: addComponent<T>(entity, component)
- New: addComponent<T>(entity, std::make_shared<T>(...))

Relates to #56
```

## Advanced Git Techniques

### Interactive Rebase

Clean up commit history before PR:

```bash
# Rebase last 3 commits
git rebase -i HEAD~3

# In editor, choose actions:
# pick = keep commit
# reword = change message
# squash = combine with previous
# drop = remove commit

# Save and close editor
# Follow prompts if needed
```

### Cherry-Pick Commits

Apply specific commits from another branch:

```bash
# Copy commit from another branch
git cherry-pick <commit-hash>

# Multiple commits
git cherry-pick <hash1> <hash2> <hash3>

# Range of commits
git cherry-pick <start-hash>..<end-hash>
```

### Bisect (Find Bug)

Binary search to find which commit introduced a bug:

```bash
# Start bisect
git bisect start

# Mark current as bad
git bisect bad

# Mark known good commit
git bisect good <commit-hash>

# Git checks out middle commit
# Test and mark as good or bad
git bisect good  # or bad

# Repeat until found
# Git will identify the commit

# Reset when done
git bisect reset
```

### Worktrees

Work on multiple branches simultaneously:

```bash
# Create worktree
git worktree add ../r-type-feature feature/new-feature

# Now you have two working directories
cd ../r-type-feature

# Remove worktree when done
git worktree remove ../r-type-feature
```

## Git Configuration

### User Setup

```bash
# Set name and email
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"

# Check configuration
git config --list
```

### Useful Aliases

```bash
# Add aliases to ~/.gitconfig
git config --global alias.st status
git config --global alias.co checkout
git config --global alias.br branch
git config --global alias.ci commit
git config --global alias.lg "log --graph --oneline --all"
git config --global alias.unstage "restore --staged"

# Use aliases
git st        # git status
git co main   # git checkout main
git lg        # pretty log
```

### Editor Configuration

```bash
# Set default editor
git config --global core.editor "vim"
# or
git config --global core.editor "code --wait"  # VS Code
```

## Troubleshooting

### Forgot to Create Branch

```bash
# Made commits on main by mistake
git checkout -b feature/my-changes
git checkout main
git reset --hard origin/main
git checkout feature/my-changes
```

### Wrong Branch

```bash
# Made commits on wrong branch
git stash
git checkout correct-branch
git stash pop
```

### Merge Conflicts

```bash
# During merge
git status  # See conflicting files

# Edit files, remove conflict markers
# <<<<<<<, =======, >>>>>>>

# After resolving
git add resolved_files
git merge --continue

# Or abort and try again
git merge --abort
```

### Accidentally Committed Secrets

```bash
# Remove file from history (BE CAREFUL!)
git filter-branch --tree-filter 'rm -f secrets.json' HEAD

# Better: use BFG Repo-Cleaner
# https://rtyley.github.io/bfg-repo-cleaner/

# Then force push
git push --force
```

### Large File Issues

```bash
# Remove large file from history
git filter-branch --tree-filter 'rm -rf large_file.bin' HEAD

# Or use Git LFS for large files
git lfs install
git lfs track "*.bin"
```

## Resources

- [Git Documentation](https://git-scm.com/doc)
- [GitHub Flow Guide](https://guides.github.com/introduction/flow/)
- [Conventional Commits](https://www.conventionalcommits.org/)
- [Oh Shit, Git!?!](https://ohshitgit.com/)

---

Following this workflow ensures smooth collaboration and maintains code quality!
