---
sidebar_position: 1
---

# Package Managers Overview

This section contains Proofs of Concept (POCs) testing different C++ package managers for managing dependencies in the R-Type project. We evaluated three major solutions: **vcpkg**, **Conan**, and **CPM**.

## Why Package Managers?

Modern C++ projects often rely on numerous external libraries. Managing these dependencies manually is error-prone and time-consuming. Package managers automate:

- Dependency resolution and downloading
- Library compilation and caching
- Cross-platform compatibility
- Version management
- Transitive dependency handling

## Test Case: SFML

All POCs use **SFML (Simple and Fast Multimedia Library)** as a test dependency because:

- It's representative of typical game development libraries
- It has multiple system dependencies (OpenGL, audio, etc.)
- It's cross-platform (Windows, Linux, macOS)
- It's well-supported by all three package managers

## POCs Structure

Each POC demonstrates:

1. Initial setup and installation
2. Dependency declaration
3. CMake integration
4. Build process
5. Real-world compilation experience
6. Pros and cons analysis

## Available POCs

### [vcpkg](./vcpkg.md)
Microsoft's C++ package manager with binary caching and manifest mode.

### [Conan](./conan.md)
Flexible package manager with Python-based recipes and multiple remotes support.

### [CPM](./cpm.md)
CMake-based lightweight package manager that builds from source.

### [Comparison](./comparison.md)
Detailed side-by-side comparison to help choose the right tool for your project.
