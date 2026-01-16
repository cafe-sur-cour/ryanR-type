---
sidebar_position: 98
---

# Contribution Guide

Welcome to the R-Type project! This guide will help new contributors understand how to contribute effectively.

## Repository Structure
- The repository is organized by feature and system (see the root and docs for details).
- Main folders include: `client/`, `server/`, `common/`, `frontend/`, `tests/`, `scripts/`, and `documentation/`.
- The `main` branch is protected. All changes must go through feature/fix branches and pull requests.

## Branching Strategy
- Create a new branch for each feature, fix, or documentation update.
- Only one feature/fix per branch and per PR.
- Branches use **kebab-case** (e.g., `add-doc`, `fix-network-bug`).
- Always link your branch to a corresponding issue.

## Commit Message Conventions
- Use **Conventional Commits** (e.g., `feat: add leaderboard`, `fix: correct packet loss`, `docs: update README`).
- Keep messages concise and descriptive.

## How to Run the Project
- See the existing documentation for instructions on running the client and server.
- Scripts are available in the `scripts/` folder (e.g., `compile_project.sh`, `run_unit_tests.sh`).

## How to Write Tests
- Tests are located in the `tests/` folder.
- Follow the structure and examples provided in existing test files.
- Use the provided scripts to run tests (`./scripts/run_unit_tests.sh`).

## Code Style Conventions
- See the [Team Conventions](./TEAM_CONVENTIONS.md) page for code style, naming, and review rules.

## How to Submit PRs and Reviews
- Open a pull request for your branch.
- Assign at least 2 reviewers.
- Link the PR to the relevant issue and add appropriate labels (e.g., `issue`, `release:patch`, `release:minor`, `release:major`).
- PRs must be reviewed and approved before merging.

---

For any questions, open an issue or ask the team for help.
