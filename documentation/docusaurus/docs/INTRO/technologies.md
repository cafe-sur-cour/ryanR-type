---
sidebar_position: 5
---

# Technologies and Tools

Overview of the technologies, libraries, and tools used in Ryan R-Type.

## 💻 Programming Languages

### C++20
The entire project is built using modern C++20, leveraging:
- **Concepts**: Type constraints for templates
- **Ranges**: Functional-style data manipulation
- **Coroutines**: Asynchronous programming (future enhancement)
- **Modules**: Faster compilation (experimental)
- **std::span**: Safe array views
- **std::format**: Type-safe string formatting
- **Designated initializers**: Cleaner struct initialization

**Why C++20?**
- High performance for game loops
- Low-level control over memory
- Rich ecosystem of libraries
- Cross-platform compatibility

## 🏗️ Build System

### CMake 3.23+
Modern CMake features used:
- **Presets**: Standardized build configurations
- **FetchContent**: Dependency management
- **Target-based**: Modern CMake practices
- **Cross-platform**: Windows, Linux, macOS support

```cmake
# CMake presets
cmake --preset unix-release
cmake --preset windows-release
```

### vcpkg
Microsoft's C++ package manager for:
- SFML (graphics)
- ASIO (networking)
- nlohmann/json (JSON parsing)
- doctest (unit testing)

**Benefits:**
- Reproducible builds
- Version control for dependencies
- Cross-platform package management

## 📚 Core Libraries

### SFML (Simple and Fast Multimedia Library)

**Version:** 2.6+  
**Purpose:** Graphics, audio, and window management

**Features used:**
- `sf::RenderWindow`: Window creation and rendering
- `sf::Sprite`: 2D texture rendering
- `sf::Shader`: GLSL shaders for effects
- `sf::Music` / `sf::Sound`: Audio playback
- `sf::Font` / `sf::Text`: Text rendering

**Why SFML?**
- Easy to use API
- Cross-platform
- Active community
- Good performance for 2D games

### ASIO (Asynchronous I/O)

**Version:** Standalone ASIO (non-Boost)
**Purpose:** Network communication

**Features used:**
- `asio::io_context`: Asynchronous I/O operations
- `asio::ip::udp::socket`: UDP networking
- `asio::ip::tcp::socket`: TCP for HTTP server
- `asio::steady_timer`: Timing operations

**Why ASIO?**
- Header-only library option
- Excellent performance
- Async/await patterns
- Cross-platform networking

### nlohmann/json

**Version:** 3.11+
**Purpose:** JSON parsing and serialization

**Usage:**
- Configuration files
- Entity definitions (prefabs)
- Map data
- Save files
- HTTP API responses

**Why nlohmann/json?**
- Modern C++ API
- Intuitive syntax
- Comprehensive features
- Well-documented

```cpp
// Example usage
json config = json::parse(file);
std::string ip = config["server"]["ip"];
```

## 🧪 Testing Framework

### Doctest

**Purpose:** Unit and integration testing

**Features:**
- Fast compilation
- Minimal header-only library
- BDD-style assertions
- Test fixtures
- Parameterized tests

```cpp
TEST_CASE("Vector addition") {
    Vector2D a{1, 2};
    Vector2D b{3, 4};
    REQUIRE(a + b == Vector2D{4, 6});
}
```

**Test coverage areas:**
- ECS components and systems
- Network packet serialization
- Collision detection
- Game logic
- Utility functions

## 🔧 Development Tools

### Version Control

**Git** with GitHub:
- Source code management
- Collaborative development
- Issue tracking
- Pull requests
- GitHub Actions for CI/CD

### Continuous Integration

**GitHub Actions:**
- Automated builds on push
- Multi-platform testing (Linux, Windows, macOS)
- Code quality checks
- Test execution
- Documentation deployment

```yaml
# Example workflow
name: Build
on: [push, pull_request]
jobs:
  build:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, windows-latest, macos-latest]
```

### Documentation

**Docusaurus:**
- Static site generator
- React-based
- Markdown documentation
- Versioning support
- Search functionality
- Mobile-responsive

**Doxygen:**
- API documentation generation
- UML diagrams
- Call graphs
- PDF export
- Code navigation

### Code Quality

**Tools used:**
- **clang-format**: Code formatting
- **clang-tidy**: Static analysis
- **cppcheck**: Additional linting
- **valgrind**: Memory leak detection (Linux)
- **AddressSanitizer**: Runtime error detection

## 🎨 Asset Creation

### Graphics
- **Sprites**: PNG with alpha transparency
- **Animations**: Sprite sheets
- **Fonts**: TrueType fonts (TTF)
- **Shaders**: GLSL for visual effects

### Audio
- **Music**: OGG Vorbis format
- **Sound Effects**: WAV format
- **Audio Editing**: Audacity

### Design
- **Sprite Editor**: Aseprite
- **Image Editor**: GIMP
- **Vector Graphics**: Inkscape

## 🌐 Networking

### Protocol
- **UDP**: Low-latency game state updates
- **Custom Protocol**: PSJM (Project Synchronization J&M)
- **Magic Number**: 0x93 for packet validation
- **Serialization**: Manual binary packing

### HTTP API
- **Beast** (part of Boost): HTTP server
- **RESTful**: Standard HTTP methods
- **JSON**: Response format
- **CORS**: Cross-origin support

## 📦 Project Structure

### Architecture Pattern

**Entity-Component-System (ECS):**
```
Entity: uint32_t ID
Component: Data-only structs
System: Logic functions operating on components
```

**Design Patterns:**
- **Singleton**: Resource managers
- **Factory**: Entity creation
- **Observer**: Event system
- **Strategy**: Platform abstraction

### Dynamic Libraries

**Platform-specific implementations:**
- `libs/Multimedia/`: SFML wrapper
- `libs/Network/`: ASIO wrapper
- `libs/Packet/`: Serialization

**Benefits:**
- Hot-reloading (development)
- Platform abstraction
- Modular codebase
- Plugin architecture

## 🐳 Deployment

### Docker
```dockerfile
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y \
    build-essential cmake
COPY . /app
WORKDIR /app
RUN ./scripts/compile_project.sh
CMD ["./r-type_server"]
```

### Package Formats
- **Linux**: AppImage, DEB, RPM
- **Windows**: ZIP, Installer (NSIS)
- **macOS**: DMG, APP bundle

## 🔐 Security

### Libraries
- **OpenSSL**: Future encryption support
- **libsodium**: Cryptographic operations

### Practices
- Input validation
- Buffer overflow protection
- Safe integer operations
- Memory safety (smart pointers)

## 📊 Performance Profiling

### Tools
- **perf** (Linux): CPU profiling
- **Instruments** (macOS): Performance analysis
- **Visual Studio Profiler** (Windows)
- **Tracy Profiler**: Real-time profiling

### Metrics Tracked
- Frame time
- Tick time
- Network latency
- Memory usage
- Cache misses

## 🌍 Cross-Platform Compatibility

### Operating Systems
- **Linux**: Ubuntu 20.04+, Fedora 35+, Arch
- **Windows**: Windows 10+, Windows 11
- **macOS**: macOS 11 (Big Sur)+

### Architectures
- **x86_64**: Primary support
- **ARM64**: macOS M1/M2 support

### Compilers
- **GCC**: 11.0+
- **Clang**: 12.0+
- **MSVC**: 2019+

## 📚 Learning Resources

### Documentation
- [SFML Documentation](https://www.sfml-dev.org/documentation/)
- [ASIO Documentation](https://think-async.com/Asio/asio-1.22.0/doc/)
- [CMake Documentation](https://cmake.org/documentation/)
- [C++20 Reference](https://en.cppreference.com/)

### Books
- "Game Programming Patterns" by Robert Nystrom
- "Effective Modern C++" by Scott Meyers
- "Computer Networking: A Top-Down Approach"

---

This technology stack was carefully chosen to balance performance, maintainability, and developer experience while meeting the project's technical requirements.
