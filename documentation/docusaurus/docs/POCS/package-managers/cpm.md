---
sidebar_position: 4
---

# CPM POC

**CPM (CMake Package Manager)** is a lightweight, CMake-native package manager that downloads and builds dependencies from source.

## Overview

CPM.cmake is a single-file solution that integrates directly into your CMakeLists.txt. It downloads dependencies from Git repositories (typically GitHub) and builds them as part of your project.

## Installation

### Prerequisites
- **CMake** 3.21+
- **Git**
- **C++17 compiler** (GCC, Clang, MSVC)
- **SFML system dependencies** (OpenGL, X11, etc. on Linux)

### System Dependencies (Linux)

```bash
# Ubuntu/Debian
sudo apt install -y build-essential cmake git \
    libx11-dev libxrandr-dev libxcursor-dev libxi-dev \
    libudev-dev libgl1-mesa-dev libfreetype6-dev \
    libopenal-dev libflac-dev libvorbis-dev

# Fedora/RHEL
sudo dnf install -y cmake gcc-c++ git \
    libX11-devel libXrandr-devel libXcursor-devel libXi-devel \
    systemd-devel mesa-libGL-devel freetype-devel \
    openal-soft-devel flac-devel libvorbis-devel
```

### Setup

**No installation required!** Just download a single file:

```bash
mkdir -p cmake
curl -fsSL https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/CPM.cmake \
    -o cmake/CPM.cmake
```

## Project Configuration

### CMakeLists.txt Integration

```cmake
cmake_minimum_required(VERSION 3.21)
project(sfml_poc)

# Download CPM.cmake if not present
set(CPM_DOWNLOAD_VERSION 0.38.1)
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake")
    file(DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake"
    )
endif()

# Include CPM
include(cmake/CPM.cmake)

# Add SFML dependency
CPMAddPackage(
    NAME SFML
    GITHUB_REPOSITORY SFML/SFML
    GIT_TAG 2.6.1
    OPTIONS
        "SFML_BUILD_AUDIO TRUE"
        "SFML_BUILD_GRAPHICS TRUE"
        "SFML_BUILD_WINDOW TRUE"
        "SFML_BUILD_NETWORK FALSE"
)

# Your executable
add_executable(sfml_poc main.cpp)
target_link_libraries(sfml_poc PRIVATE sfml-graphics sfml-window sfml-system)
```

### Key Features

- **Automatic download**: CPM.cmake downloads itself if missing
- **Git-based**: Dependencies fetched from Git repos
- **Version pinning**: Use Git tags, branches, or commits
- **CMake options**: Configure dependencies directly

## Build Process

```bash
# Configure (downloads and builds dependencies)
cmake --preset cpm-release

# Build
cmake --build --preset cpm-release

# Run
./bin/sfml_poc
```

**First build:** Downloads sources and compiles everything (can be slow).

**Subsequent builds:** Uses CMake cache (faster if no clean).

## How CPM Works

1. **Download CPM.cmake**: Single-file script in `cmake/` directory
2. **Include in CMake**: `include(cmake/CPM.cmake)`
3. **Declare dependencies**: `CPMAddPackage()` with GitHub repo and version
4. **Automatic download**: CPM clones repos to `build/_deps/`
5. **Configuration**: Dependencies configured as CMake subprojects
6. **Build**: Everything compiled together

## CPM Cache

CPM uses a global cache to avoid re-downloading dependencies:

```bash
# Default cache location
$HOME/.cache/CPM/

# Force re-download
rm -rf build/_deps/
```

## Customization

### Change SFML Version

```cmake
CPMAddPackage(
    NAME SFML
    GITHUB_REPOSITORY SFML/SFML
    GIT_TAG 2.6.1  # Specific version
)
```

### Add Other Dependencies

```cmake
# Example: Add spdlog
CPMAddPackage(
    NAME spdlog
    GITHUB_REPOSITORY gabime/spdlog
    VERSION 1.12.0
)
```

### Use Custom Git Repositories

```cmake
CPMAddPackage(
    NAME MyLibrary
    GIT_REPOSITORY https://gitlab.com/user/repo.git
    GIT_TAG main
)
```

## Real-World Experience

### First Build
- ⚠️ **Slow**: Compiles all dependencies from source (10-15 minutes for SFML)
- ✅ **Simple**: No external tools needed
- ✅ **Clean**: CMake-native integration

### Subsequent Builds
- ✅ **Faster**: CMake cache reduces rebuilds
- ⚠️ **Still slow after clean**: Recompiles everything

### System Dependencies
- ⚠️ Requires system libraries (OpenGL, X11, audio libs)
- ✅ Works if system deps are installed

## Strengths

### ✅ Ultimate Simplicity
- **Zero installation**: Just one file to download
- **No external tools**: Only CMake and Git required
- **CMake-native**: No toolchain files or special setup

### ✅ Full Source Control
- Always builds from source
- Guaranteed compatibility with your compiler
- Full control over build flags

### ✅ Flexibility
- Works with any public Git repository
- Supports GitHub, GitLab, Bitbucket, custom Git servers
- Version control via Git tags/branches/commits

### ✅ Global Cache
- Shared cache in `~/.cache/CPM/`
- Avoids re-downloading same dependencies
- Saves disk space

### ✅ Ideal for Prototypes
- Fastest setup time (no installation)
- Perfect for POCs and experiments
- Great for educational projects

## Weaknesses

### ❌ Build Time
- **Very slow first build**: Compiles everything from source
- No pre-compiled binaries
- 10-20 minutes typical for medium projects

### ❌ Rebuild After Clean
- `cmake --build . --target clean` loses everything
- Must recompile all dependencies
- Not ideal for CI/CD without caching

### ❌ System Dependencies
- Still requires system libraries (OpenGL, X11, etc.)
- Not fully self-contained
- Platform-specific setup needed

### ❌ Limited Ecosystem
- No centralized package registry
- Depends on GitHub/Git availability
- Private repositories more complex

### ❌ No Binary Caching
- Can't share pre-built binaries between machines
- Every developer compiles from scratch
- Slower for large teams

### ❌ Advanced Features
- No lockfiles (by default)
- No dependency conflict resolution
- Limited versioning compared to vcpkg/Conan

## When to Use CPM

**Ideal for:**
- ✅ Quick prototypes and POCs
- ✅ Projects with few dependencies
- ✅ Learning/educational projects
- ✅ When you want simplest possible setup
- ✅ Dependencies available on GitHub
- ✅ Full source builds preferred

**Not recommended for:**
- ❌ Projects with many dependencies
- ❌ CI/CD requiring fast builds
- ❌ Large teams (compilation time multiplied)
- ❌ Production projects with tight deadlines

## Comparison with Other Tools

| Aspect | CPM | vcpkg | Conan |
|--------|-----|-------|-------|
| Installation | ⭐⭐⭐⭐⭐ One file | ⭐⭐⭐⭐ Bootstrap | ⭐⭐⭐ pip |
| Binaries | ❌ Never | ✅ Usually | ⚠️ Variable |
| First Build | ⭐⭐ Slow | ⭐⭐⭐⭐⭐ Fast | ⭐⭐⭐⭐ Medium |
| Simplicity | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐ |
| Flexibility | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| Production | ⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ |

## POC Verdict

✅ **Successful compilation** but with trade-offs:
- Setup was trivial (no installation needed)
- First build took ~15 minutes (SFML from source)
- Works perfectly for this simple POC
- Would be problematic for larger projects

**Recommendation:** ⭐ **Perfect for prototypes, not for production.**

## Best Practices

### CI/CD Optimization

```yaml
# Cache CPM dependencies in CI
- name: Cache CPM
  uses: actions/cache@v3
  with:
    path: ~/.cache/CPM
    key: ${{ runner.os }}-cpm-${{ hashFiles('**/CMakeLists.txt') }}
```

### Version Pinning

Always use specific versions (Git tags) for reproducibility:

```cmake
CPMAddPackage(
    NAME SFML
    GITHUB_REPOSITORY SFML/SFML
    GIT_TAG 2.6.1  # ✅ Specific version
    # GIT_TAG master  # ❌ Avoid for production
)
```

## Troubleshooting

### Error: "Could not find Git"
```bash
sudo apt install git  # Linux
```

### SFML compilation errors
```bash
# Install system dependencies
sudo apt install libx11-dev libxrandr-dev libgl1-mesa-dev libfreetype6-dev
```

### Corrupted CPM cache
```bash
rm -rf ~/.cache/CPM/
rm -rf build/
```

## Conclusion

CPM is an excellent choice for the R-Type project **during prototyping phase**:
- ✅ Zero setup friction
- ✅ Perfect for POCs and experimentation
- ✅ Good for learning CMake integration

However, for production, consider switching to vcpkg or Conan to benefit from:
- Pre-compiled binaries
- Faster CI/CD builds
- Better team scalability

## Resources

- [CPM.cmake GitHub](https://github.com/cpm-cmake/CPM.cmake)
- [CPM Documentation](https://github.com/cpm-cmake/CPM.cmake/wiki)
- [Example Projects](https://github.com/cpm-cmake/CPM.cmake/wiki/More-Snippets)
