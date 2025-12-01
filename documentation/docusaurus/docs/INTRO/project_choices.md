---
sidebar_position: 3
---

# Project Choices

This document outlines the key architectural and technological decisions made for the R-Type project, based on extensive Proof of Concepts (POCs) and team evaluation.

## Architecture Overview

R-Type is implemented as a multiplayer 2D shoot'em up game with a client-server architecture. The project emphasizes modularity, performance, and maintainability through modern C++ practices and proven design patterns.

## Core Architecture Choices

### Entity Component System (ECS)

**Choice: Component-Driven ECS with Tags/Filters Optimization**

Based on our ECS POCs, we adopted a **Component-Driven** approach as the primary ECS implementation. This pattern offers the best balance of modularity, cleanliness, and simplicity for our team's needs.

**Key Features:**
- Systems communicate through temporary effect components
- Clean separation between data (components) and logic (systems)
- Easy to extend and modify without affecting other parts
- Complemented with **Tags/Filters** for performance optimization in entity queries

**Benefits:**
- High modularity and maintainability
- Flexible for various game mechanics
- Efficient processing with tag-based filtering
- Suitable for real-time game requirements

### Scene Management

**Choice: Game State Machine (GSM) with ECS Integration**

The project uses a **Game State Machine** to manage different game scenes (Menu, Gameplay, Pause, etc.) with seamless ECS integration.

**Key Features:**
- Clean state lifecycle (enter/update/render/exit)
- Automatic resource cleanup between state transitions
- Each state manages its own entities and systems
- Modular game logic organization

**Benefits:**
- Clear separation of game logic
- Prevents memory leaks through automatic cleanup
- Easy to add new game states
- Integrated with ECS for entity management per state

### Dynamic Library Loading

**Choice: Runtime Library Loading with DLLoader**

The project implements dynamic loading of core libraries (Network, Buffer, Packet) at runtime using a custom DLLoader system.

**Key Features:**
- Platform-independent library loading
- Runtime flexibility for different implementations
- Clean separation of concerns
- Error handling for missing libraries

**Benefits:**
- Modular architecture allowing different library implementations
- Easier testing and development
- Platform compatibility
- Reduced binary size through optional components

## Technology Stack

### Graphics Library

**Choice: SFML (Simple and Fast Multimedia Library)**

After evaluating SFML, SDL2, and Raylib, we selected **SFML** for our graphics rendering needs.

**Rationale:**
- **Object-oriented API**: Consistent and intuitive classes
- **Automatic resource management**: RAII prevents memory leaks
- **Optimized 2D rendering**: Batching and vertex arrays for game entities
- **Complete ecosystem**: Rendering, audio, networking, and system APIs
- **Cross-platform support**: Windows, Linux, macOS, and extended platforms
- **Modern C++ integration**: Smart pointers, exceptions
- **Team familiarity**: Pleasant to use and well-documented

**Benefits:**
- Excellent performance for 2D games
- Rich feature set for game development
- Strong community and documentation
- Scalable for medium to large projects

### Networking

**Choice: UDP with Custom Reliability Layer (Handshake)**

For network communications, we chose **UDP with a custom handshake mechanism** for critical communications, while maintaining UDP for non-critical data.

**Rationale:**
- **Low latency**: Essential for responsive multiplayer gameplay
- **Custom reliability**: Ensures critical game state delivery
- **Unified protocol**: Single UDP-based stack for all communications
- **Performance optimization**: Reduced overhead compared to TCP

**Key Features:**
- Handshake protocol for critical message acknowledgment
- Sequence numbering and retransmission
- Security measures against network attacks
- Optimized for real-time shooter mechanics

### JSON Handling

**Choice: nlohmann/json Library**

We selected the **nlohmann/json** library for JSON parsing and serialization needs.

**Rationale:**
- **Header-only**: No separate compilation required
- **Modern C++**: Uses latest language features
- **Intuitive API**: Easy to use for parsing and generating JSON
- **Comprehensive**: Handles all JSON data types
- **Well-maintained**: Active development and community support

**Use Cases:**
- Configuration files
- Network message serialization
- Save game data
- Asset metadata

### Package Management

**Choice: vcpkg**

After comparing vcpkg, Conan, and CPM, we selected **vcpkg** as our package manager.

**Rationale:**
- **Fast builds**: Pre-compiled binaries for quick iteration
- **Easy setup**: Simple bootstrap and configuration
- **Cross-platform**: Excellent Windows and Linux support
- **Reliable**: Works well with modern compilers
- **Team-friendly**: Low learning curve and clear documentation

**Benefits:**
- Rapid development cycle with cached binaries
- Consistent dependency management across platforms
- Strong ecosystem with 2000+ packages
- Active Microsoft support and maintenance

## Build System and Dependencies

### Build System

**Choice: CMake with Presets**

The project uses **CMake** as the build system with modern presets for different configurations.

**Key Features:**
- Cross-platform build generation
- Preset configurations for different environments
- Modular CMakeLists.txt structure
- Integration with vcpkg toolchain

### Core Dependencies

Based on our technology choices, the project depends on:

- **SFML**: Graphics, audio, and input handling
- **Asio**: Networking library for TCP/UDP communications
- **GTest**: Unit testing framework
- **nlohmann/json**: JSON handling (via FetchContent in CMake)

### Compiler Standards

- **C++20**: Modern language features and performance
- **Strict compilation**: High warning levels and treated as errors
- **Cross-platform compatibility**: GCC, Clang, and MSVC support

## Development Practices

### Code Quality

- **Static analysis**: Comprehensive warning flags
- **Unit testing**: GTest integration for test-driven development
- **Documentation**: Doxygen for API documentation
- **Code style**: Consistent formatting and naming conventions

### Project Structure

- **Modular organization**: Separate directories for client, server, common, and libraries
- **Asset management**: Embedded assets for deployment
- **Build optimization**: Separate build configurations for different targets

## Conclusion

The project choices were made through systematic evaluation of alternatives via Proof of Concepts. Each decision balances technical requirements with team capabilities and project constraints.

**Key Success Factors:**
- **Performance**: UDP networking and optimized ECS for real-time gameplay
- **Maintainability**: Component-driven ECS and modular architecture
- **Developer Experience**: Familiar technologies (SFML, CMake) and fast builds (vcpkg)
- **Scalability**: Extensible systems for future feature additions

These choices provide a solid foundation for developing a high-quality multiplayer game while maintaining development efficiency and code quality.
