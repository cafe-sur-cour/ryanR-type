---
sidebar_position: 2
---

# vcpkg POC

**vcpkg** is Microsoft's C/C++ package manager that provides a large catalog of open-source libraries with pre-compiled binaries.

## Overview

vcpkg uses a centralized registry approach with binary caching, making it particularly fast for common configurations. It integrates seamlessly with CMake through a toolchain file.

## Installation

### Prerequisites
- **CMake** 3.21+
- **C++17 compiler** (GCC, Clang, MSVC)
- **Git**

### Setup

```bash
# Clone vcpkg repository
git clone https://github.com/Microsoft/vcpkg.git

# Bootstrap vcpkg
# Linux/macOS
cd vcpkg
./bootstrap-vcpkg.sh

# Windows
cd vcpkg
.\bootstrap-vcpkg.bat

# Set environment variable
export VCPKG_ROOT=/path/to/vcpkg  # Linux/macOS
set VCPKG_ROOT=C:\path\to\vcpkg   # Windows CMD
```

## Project Configuration

### Manifest Mode (Recommended)

vcpkg supports **manifest mode** where dependencies are declared in a `vcpkg.json` file:

```json
{
  "dependencies": ["sfml"]
}
```

This file is checked into version control, making dependency management reproducible across the team.

### CMakePresets.json

Using CMake presets simplifies the build configuration:

```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "vcpkg-release",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build/release",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release",
        "CMAKE_TOOLCHAIN_FILE": "$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      }
    }
  ],
  "buildPresets": [
    {
      "name": "vcpkg-release",
      "configurePreset": "vcpkg-release"
    }
  ]
}
```

## Build Process

```bash
# Configure with automatic dependency installation
cmake --preset vcpkg-release

# Build
cmake --build --preset vcpkg-release

# Run
./bin/sfml_poc
```

**Key advantage:** Dependencies are installed **automatically** during CMake configuration when using manifest mode.

## Features Demonstrated

The POC creates a simple SFML window that:
- Detects the operating system (Linux/Windows)
- Opens a window titled "SFML Test - [OS]"
- Handles window close events
- Closes on Escape key press

## Real-World Experience

### First Build
- ✅ **Fast installation**: Pre-compiled binaries downloaded in seconds
- ✅ **Automatic dependencies**: SFML and all its dependencies installed automatically
- ✅ **Clean output**: Minimal console output, easy to follow

### Subsequent Builds
- ✅ **Very fast**: Shared cache between projects
- ✅ **Reliable**: Consistent behavior across machines

### System Dependencies
- ⚠️ On Linux, system dependencies (OpenGL, X11, etc.) must be installed separately
- ✅ On Windows, everything is self-contained

## Strengths

### ✅ Ease of Use
- Simple installation (one bootstrap script)
- No external dependencies (doesn't require Python)
- Clear, centralized documentation

### ✅ Performance
- Pre-compiled binaries for common configurations
- Shared cache between all projects
- Very fast subsequent builds

### ✅ Integration
- Native CMake integration via toolchain file
- Manifest mode with `vcpkg.json`
- Automatic dependency installation
- Excellent Visual Studio integration

### ✅ Ecosystem
- 2000+ packages in the registry
- Official Microsoft support
- GitHub Actions integration
- Active community

## Weaknesses

### ❌ Flexibility
- Less customizable than Conan
- Multi-configuration builds require triplets
- Limited control over build process

### ❌ Configuration
- Toolchain file must be specified manually (or via presets)
- Triplets can be confusing for beginners
- Private registries more complex to set up

### ❌ Binary Availability
- Binaries not always available for all configurations
- Falls back to source build (can be slow)
- May need custom triplets for unusual targets

## When to Use vcpkg

**Ideal for:**
- ✅ Windows/Visual Studio projects
- ✅ Teams new to C++ package managers
- ✅ Projects needing fast builds
- ✅ Microsoft ecosystem integration
- ✅ Standard configurations (common OS/compiler combinations)

**Less suitable for:**
- ❌ Complex cross-compilation scenarios
- ❌ Projects requiring extensive build customization
- ❌ Embedded systems with unusual targets

## Comparison with Other Tools

| Aspect | vcpkg | Conan | CPM |
|--------|-------|-------|-----|
| Installation | Bootstrap script | pip install | Single file |
| Binaries | ✅ Usually available | ⚠️ Variable | ❌ Always source |
| Speed | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐ |
| Flexibility | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ |
| Simplicity | ⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ |

## Conclusion

vcpkg offers an excellent balance between simplicity and functionality. It's particularly well-suited for the R-Type project if:

- Fast iteration is important
- The team uses Windows/Visual Studio
- Standard dependencies are sufficient
- Pre-compiled binaries are available for your platform

The manifest mode and CMake preset integration make it easy to maintain consistent builds across the team.

## Resources

- [Official vcpkg Documentation](https://vcpkg.io/)
- [vcpkg GitHub Repository](https://github.com/microsoft/vcpkg)
- [Package Search](https://vcpkg.io/en/packages.html)
