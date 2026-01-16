---
sidebar_position: 3
---

# Installation Guide

This guide covers installing and running Ryan R-Type on all supported platforms.

## 📋 System Requirements

### Minimum Requirements
- **OS**: Windows 10+, Linux (Ubuntu 20.04+), macOS 11+
- **CPU**: Dual-core 2.0 GHz
- **RAM**: 512 MB
- **Graphics**: OpenGL 3.3 compatible GPU
- **Network**: Internet connection for multiplayer
- **Storage**: 100 MB available space

### Recommended Requirements
- **OS**: Windows 11, Linux (Ubuntu 22.04+), macOS 13+
- **CPU**: Quad-core 3.0 GHz
- **RAM**: 2 GB
- **Graphics**: Dedicated GPU with OpenGL 4.5
- **Network**: Broadband internet connection
- **Storage**: 500 MB available space

## 🚀 Quick Start

### Pre-built Binaries (Recommended)

Download the latest release for your platform from the [releases page](https://github.com/AlbanRSS/doc-rtype/releases).

#### Windows

1. Download `r-type-windows-x64.zip`
2. Extract to your preferred location
3. Run `r-type_server.exe` to start the server
4. Run `r-type_client.exe` to start the client

#### Linux

```bash
# Download and extract
wget https://github.com/AlbanRSS/doc-rtype/releases/latest/download/r-type-linux-x64.tar.gz
tar -xzf r-type-linux-x64.tar.gz
cd r-type

# Run server
./r-type_server

# Run client (in another terminal)
./r-type_client -n Player1
```

#### macOS

```bash
# Download and extract
curl -LO https://github.com/AlbanRSS/doc-rtype/releases/latest/download/r-type-macos-x64.tar.gz
tar -xzf r-type-macos-x64.tar.gz
cd r-type

# Run server
./r-type_server

# Run client (in another terminal)
./r-type_client -n Player1
```

## 🔨 Building from Source

### Prerequisites

#### All Platforms
- **C++ Compiler** with C++20 support:
  - GCC 11+ (Linux)
  - Clang 12+ (macOS)
  - MSVC 2019+ (Windows)
- **CMake**: Version 3.23 or higher
- **vcpkg**: Package manager for dependencies
- **Git**: Version control system

#### Installing Prerequisites

**Windows:**
```cmd
# Install Visual Studio 2019+ with C++ workload
# Install CMake from https://cmake.org/download/
# Install Git from https://git-scm.com/

# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
```

**Linux (Ubuntu/Debian):**
```bash
# Install compiler and build tools
sudo apt update
sudo apt install -y build-essential cmake git pkg-config

# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
```

**macOS:**
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake git

# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
```

### Building the Project

#### Unix Systems (Linux/macOS)

```bash
# Clone the repository
git clone https://github.com/AlbanRSS/doc-rtype.git
cd R-Type

# Install dependencies
./scripts/install_dependencies.sh

# Build the project
./scripts/compile_project.sh

# Binaries will be in the build directory
ls build/unix/
```

#### Windows

```cmd
REM Clone the repository
git clone https://github.com/AlbanRSS/doc-rtype.git
cd R-Type

REM Install dependencies
.\scripts\install_dependencies.bat

REM Build the project
.\scripts\compile_project.bat

REM Binaries will be in the build directory
dir build\windows\
```

### Build Options

The project supports various build configurations:

```bash
# Debug build (with symbols and debugging info)
cmake --preset unix-debug
cmake --build build/unix-debug

# Release build (optimized)
cmake --preset unix-release
cmake --build build/unix-release

# With tests
cmake --preset unix-release -DBUILD_TESTS=ON
cmake --build build/unix-release
ctest --test-dir build/unix-release
```

## 🎮 Running the Game

### Server

```bash
# Default configuration (127.0.0.1:4242)
./r-type_server

# Custom port and IP
./r-type_server -p 8080 -i 0.0.0.0

# With custom TPS and debug mode
./r-type_server -p 4242 -i 127.0.0.1 -tps 30 -d
```

#### Server Arguments
- `-p <port>`: Port number (default: 4242)
- `-i <ip>`: IP address to bind (default: 127.0.0.1)
- `-tps <tps>`: Ticks per second (default: 20)
- `-d`: Enable debug mode
- `-h`: Display help

### Client

```bash
# Connect to localhost
./r-type_client -n Player1

# Connect to remote server
./r-type_client -p 8080 -i <server_ip> -n Alice

# Debug mode
./r-type_client -d
```

#### Client Arguments
- `-p <port>`: Server port (default: 4242)
- `-i <ip>`: Server IP (default: 127.0.0.1)
- `-n <name>`: Player name
- `-d`: Enable debug mode
- `-h`: Display help

## 🐳 Docker (Optional)

For containerized deployment:

```bash
# Build Docker image
docker build -t rtype-server .

# Run server
docker run -p 4242:4242/udp rtype-server

# Run with custom configuration
docker run -p 8080:8080/udp -e PORT=8080 -e TPS=30 rtype-server
```

## 🔧 Troubleshooting

### Common Installation Issues

#### vcpkg Dependencies Fail
```bash
# Clean vcpkg cache
rm -rf ~/.vcpkg
./scripts/install_dependencies.sh
```

#### CMake Configuration Errors
```bash
# Clear CMake cache
rm -rf build/
cmake --preset unix-release
```

#### Compiler Not Found
```bash
# Set compiler manually
export CC=/usr/bin/gcc-11
export CXX=/usr/bin/g++-11
./scripts/compile_project.sh
```

#### Missing Libraries
```bash
# Linux: Install development libraries
sudo apt install libx11-dev libxrandr-dev libxcursor-dev libxi-dev \
                 libudev-dev libgl1-mesa-dev libglu1-mesa-dev

# macOS: Install via Homebrew
brew install sfml
```

### Runtime Issues

#### Port Already in Use
```bash
# Check what's using the port
lsof -i :4242  # Linux/macOS
netstat -ano | findstr :4242  # Windows

# Use a different port
./r-type_server -p 8080
```

#### Permission Denied
```bash
# Make scripts executable
chmod +x ./scripts/*.sh

# Make binaries executable
chmod +x r-type_server r-type_client
```

#### Missing Assets
```bash
# Ensure you're in the correct directory
cd /path/to/R-Type
./r-type_server  # Run from project root
```

## 📦 Uninstallation

### Remove Binaries
```bash
# Linux/macOS
rm -rf build/
rm r-type_server r-type_client

# Windows
rmdir /s build
del r-type_server.exe r-type_client.exe
```

### Remove Dependencies
```bash
# vcpkg packages are in vcpkg/installed/
# CMake cache is in build/
# No system-wide files are installed
```

---

For additional help, see the [Troubleshooting](../INTRO/troubleshooting.md) section or open an issue on GitHub.
