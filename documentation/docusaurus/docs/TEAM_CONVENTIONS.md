---
sidebar_position: 99
---

# Team Conventions

This page documents the team conventions for code style, file organization, naming, workflow, and documentation in the R-Type project.

## Code Style Rules
- We use **cpplint** for C++ code style enforcement.
- Run the style checker using the script: `./scripts/cpp_style_checker.sh`.
- Follow cpplint guidelines for indentation, spacing, and formatting.

## File Naming & Folder Structure
- Source files use **PascalCase** or **CamelCase** (e.g., `GameRules.cpp`, `LobbyStruct.hpp`).
- Folders are organized by feature or system (e.g., `components/`, `systems/`, `packet/`).
- ECS components and systems are grouped in dedicated folders.
- Avoid generic names; prefer descriptive and specific names.

## ECS Naming Conventions
- Components: `ComponentNameComponent` (e.g., `ScoreComponent`, `HealthComponent`).
- Systems: `SystemNameSystem` (e.g., `RenderSystem`, `InputSystem`).
- Entities: Use descriptive names related to their role.

## Git Workflow
- We use **ScrumBan**: sprints + Kanban board for issues and tasks.
- Issues are tracked and assigned via GitHub.
- Branches are created for each feature, fix, or documentation update.

## Review Rules
- **Minimum 2 reviewers** per pull request.
- Reviews must be thorough and cover code quality, logic, and documentation.
- Use GitHub review tools for comments and suggestions.

## Documentation Conventions
- Documentation is written in Markdown and organized in the `documentation/docusaurus/docs/` folder.
- Use clear section headers and consistent formatting.
- Follow the structure of existing docs for new pages.

## Branch Naming
- Branches use **kebab-case** (e.g., `add-doc`, `fix-network-bug`, `update-leaderboard`).
- Names should be concise and descriptive of the change.

---

For any questions or updates to these conventions, open an issue or discuss with the team.
