---
sidebar_position: 3
---

# Conan POC

**Conan** is a decentralized, flexible C/C++ package manager with Python-based recipes and excellent support for complex configurations and cross-compilation.

## Overview

Conan 2.x offers powerful features for managing C++ dependencies with fine-grained control over build configurations, multiple remotes, and customizable build recipes.

## Installation

### Prerequisites
- **CMake** 3.21+
- **C++17 compiler** (GCC, Clang, MSVC)
- **Python 3.6+** (for Conan)

### Setup

```bash
# Install Conan via pip
pip install conan

# Verify installation
conan --version

# Create default profile
conan profile detect --force
```

:::warning Conan Version
This POC uses **Conan 2.x**. If you have Conan 1.x installed, uninstall it first as they are incompatible.
:::

## Project Configuration

### conanfile.txt

Dependencies are declared in a `conanfile.txt`:

```ini
[requires]
sfml/2.6.1

[generators]
CMakeDeps
CMakeToolchain

[options]
sfml/*:shared=True

[layout]
cmake_layout
```

**Sections explained:**
- `[requires]`: Dependencies with version specifications
- `[generators]`: CMake integration files to generate
  - `CMakeDeps`: Generates `Find*.cmake` files
  - `CMakeToolchain`: Generates toolchain with compiler flags
- `[options]`: Build options for dependencies
- `[layout]`: Build directory structure

### CMakePresets.json

```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "conan-release",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build/release",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release",
        "CMAKE_TOOLCHAIN_FILE": "${sourceDir}/build/release/generators/conan_toolchain.cmake"
      }
    }
  ]
}
```

## Build Process

```bash
# 1. Install dependencies
conan install . --output-folder=build/release --build=missing \
    -s build_type=Release \
    -c tools.system.package_manager:mode=install \
    -c tools.system.package_manager:sudo=True

# 2. Configure CMake
cmake --preset conan-release

# 3. Build
cmake --build --preset conan-release

# 4. Run
./bin/sfml_poc
```

**Key difference:** Dependencies must be installed **manually** with `conan install` before running CMake.

## Real-World Experience

### ⚠️ Critical Issues Encountered

#### 1. Missing System Dependencies
Conan requires many system libraries. On Fedora/RHEL:

```bash
sudo dnf install -y libfontenc-devel libXaw-devel libXdmcp-devel \
    libxkbfile-devel libXres-devel libXScrnSaver-devel \
    xcb-util-wm-devel xcb-util-keysyms-devel xcb-util-renderutil-devel \
    libXv-devel xcb-util-devel libuuid-devel xcb-util-cursor-devel
```

#### 2. Unsupported Modern Compilers
**Major problem:** GCC 15+, Clang 19+ often **not supported** by ConanCenter packages.

**Solution:** Use older compiler versions (GCC 11-14 recommended) or modify Conan profile:

```bash
# Edit ~/.conan2/profiles/default
# Change: compiler.version=15 → compiler.version=13
```

#### 3. Very Long Compilation Times
If pre-compiled binaries are unavailable, Conan compiles **all dependencies from source**:

- `brotli`, `bzip2`, `zlib`, `libpng`, `freetype`
- `ogg`, `vorbis`, `flac`, `openal-soft`
- `SFML` and all transitive dependencies

**Expected time:** 15-30 minutes with extremely verbose output.

#### 4. Build Failure on Modern Systems
**Actual result on Fedora 42 (GCC 15.2.1):**
- ❌ Failed compilation after 20-30 minutes
- ❌ Cryptic error messages buried in thousands of log lines
- ❌ Difficult to debug which dependency caused the failure

## Strengths

### ✅ Flexibility
- Python-based recipes for full customization
- Fine-grained control over dependencies
- Support for custom build logic

### ✅ Multi-Configuration
- Native support for Debug/Release/Custom builds
- Multiple profiles can coexist
- Easy switching between configurations

### ✅ Cross-Compilation
- Excellent native support
- Customizable profiles per target platform
- Toolchain integration

### ✅ Decentralized
- Multiple remotes (public/private)
- Easy to set up private artifactory
- Corporate-friendly architecture

### ✅ Advanced Features
- Lockfiles for reproducible builds
- Dependency overrides
- Recipe revisions and versioning
- Graph visualization

## Weaknesses

### ❌ Complexity
- Steeper learning curve than vcpkg
- More configuration files to manage
- Profile concept requires understanding

### ❌ Python Dependency
- Requires Python installation
- Can conflict with system Python
- Virtual environment recommended

### ❌ Binary Availability Issues
- Pre-compiled binaries often missing
- **Very long source compilation** (15-30+ minutes)
- **Extremely verbose build output** (thousands of lines)
- Configuration-dependent (OS/compiler/architecture)

### ❌ Modern Compiler Support
- **GCC 15+, Clang 19+ often unsupported**
- Packages built with older compiler versions
- Requires manual profile modification
- Cryptic compilation errors

### ❌ System Dependencies
- Requires many system `-devel` packages
- No automatic installation by default
- Different requirements per Linux distribution

### ❌ Debugging Difficulty
- Errors often cryptic and buried in logs
- Complex dependency chains
- Hard to identify problematic package versions

### ❌ Breaking Changes
- Conan 1.x → 2.x had major breaking changes
- Migration required for existing projects
- Some packages still on Conan 1.x

## When to Use Conan

**Ideal for:**
- ✅ Complex multi-platform projects
- ✅ Frequent cross-compilation needs
- ✅ Private package repositories
- ✅ Teams with C++ and build system expertise
- ✅ Projects requiring fine-grained build control

**Not recommended for:**
- ❌ Quick prototypes or POCs
- ❌ Modern development environments (latest compilers)
- ❌ Teams new to C++ package management
- ❌ Projects with simple dependency needs

## Comparison with Other Tools

| Aspect | Conan | vcpkg | CPM |
|--------|-------|-------|-----|
| Installation | pip (Python) | Bootstrap script | Single file |
| Binaries | ⚠️ Variable | ✅ Usually | ❌ Never |
| Speed | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐ |
| Flexibility | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐ |
| Learning Curve | ⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐ |
| Modern Compilers | ❌ Often fails | ✅ Good | ✅ Source-based |

## POC Verdict

:::danger Failed on Modern System
This POC **failed to compile** on Fedora 42 with GCC 15.2.1 after:
- Installing ~15 system dependencies
- 20-30 minutes of compilation
- Extremely verbose and difficult-to-debug output
:::

**Recommendation:** ⚠️ **Not recommended for quick starts or modern development environments.**

**Alternative:** Use **vcpkg** for faster, more reliable builds.

## When Conan Excels

Despite the issues in this POC, Conan is excellent when:
- You use older, well-supported compiler versions (GCC 11-14)
- Pre-compiled binaries are available for your configuration
- You need advanced features (cross-compilation, private repos)
- Your team has Conan expertise

## Resources

- [Official Conan Documentation](https://docs.conan.io/)
- [ConanCenter](https://conan.io/center/)
- [Conan GitHub Repository](https://github.com/conan-io/conan)

## Useful Commands

```bash
# List installed packages
conan list "*"

# Search for packages
conan search sfml --remote=conancenter

# Show package info
conan inspect sfml/2.6.1

# Clean cache
conan remove "*" --confirm

# Show profile
conan profile show default

# Force rebuild
conan install . --output-folder=build/release --build="*" -s build_type=Release
```
