---
sidebar_position: 1
---

# R-Type: A Modern Multiplayer Space Shooter

## 🎮 Project Overview

**R-Type** is a modern reimplementation of the classic 1990s arcade game, built as a multiplayer 2D space shooter using cutting-edge C++20 technologies. This project demonstrates advanced software engineering principles through a client-server architecture, real-time networking, and a custom Entity-Component-System (ECS) framework.

## 🎯 Project Goals

### Educational Objectives
- Demonstrate advanced C++ programming techniques
- Implement scalable client-server architecture
- Showcase modern game development practices
- Provide comprehensive documentation and testing

### Technical Achievements
- **Performance**: 60 FPS gameplay with efficient memory usage
- **Scalability**: Support for multiple concurrent players (up to 4 per lobby)
- **Maintainability**: Modular design with clear separation of concerns
- **Portability**: Cross-platform compatibility with minimal overhead

## 📈 Learning Outcomes

This project serves as a comprehensive example of:
- **Systems Programming**: Low-level networking and memory management
- **Game Architecture**: ECS patterns and real-time systems
- **Cross-Platform Development**: Platform abstraction and portability
- **Software Engineering**: Testing, documentation, and maintainability
- **Modern C++**: Templates, smart pointers, and C++20 language features

## 📊 Project Structure

```
R-Type/
├── client/           # 🎮 Game client implementation
│   ├── Core.cpp/hpp  # Main game loop and initialization
│   ├── systems/      # Client-side ECS systems (rendering, audio)
│   ├── components/   # Client-specific components
│   ├── gsm/          # Game State Machine
│   └── ui/           # User interface components
├── server/           # 🖥️ Game server implementation
│   ├── Server.cpp    # Server core logic
│   ├── Lobby.cpp     # Lobby management
│   ├── systems/      # Server-side ECS systems
│   └── http/         # HTTP API server
├── common/           # 🔧 Shared code between client and server
│   ├── ECS/          # Entity-Component-System framework
│   ├── components/   # Shared game components
│   ├── systems/      # Shared systems
│   ├── packet/       # Network packet definitions
│   └── utils/        # Utility functions
├── libs/             # 📚 Core libraries
│   ├── Network/      # ASIO wrapper
│   ├── Multimedia/   # SFML wrapper
│   └── Packet/       # Packet serialization
├── assets/           # 🎨 Game assets
│   ├── sprites/      # Character and object sprites
│   ├── sounds/       # Sound effects
│   ├── musics/       # Background music
│   ├── fonts/        # TrueType fonts
│   └── shaders/      # GLSL shaders (color-blind modes)
├── configs/          # ⚙️ Configuration files
│   ├── entities/     # Entity definitions
│   ├── prefab/       # Prefabricated entities
│   ├── map/          # Map configurations
│   └── rules/        # Game rules
├── tests/            # 🧪 Unit and integration tests
├── documentation/    # 📖 Project documentation
│   ├── docusaurus/   # User documentation (this site)
│   └── Doxyfile      # API documentation config
├── scripts/          # ⚙️ Build and utility scripts
│   ├── install_dependencies.sh
│   ├── compile_project.sh
│   └── generate_doc.sh
└── saves/            # 💾 Persistent game data
    ├── settings.json
    ├── keybinds.json
    ├── accessibility.json
    └── scores.json
```

## 🏗️ Architecture Overview

### Client-Server Model

```
┌─────────────────────┐         ┌─────────────────────┐
│   Game Client       │◄───────►│   Game Server       │
│                     │   UDP    │                     │
│  ┌───────────────┐  │          │  ┌───────────────┐  │
│  │ Rendering     │  │          │  │ Game Logic    │  │
│  │ System        │  │          │  │ (Authoritative)│ │
│  └───────────────┘  │          │  └───────────────┘  │
│  ┌───────────────┐  │          │  ┌───────────────┐  │
│  │ Input Handler │  │  Events  │  │ ECS Systems   │  │
│  └───────────────┘  ├─────────►│  └───────────────┘  │
│  ┌───────────────┐  │          │  ┌───────────────┐  │
│  │ Audio System  │  │  State   │  │ Lobby Manager │  │
│  └───────────────┘  │◄─────────┤  └───────────────┘  │
│  ┌───────────────┐  │          │  ┌───────────────┐  │
│  │ Local Predict │  │          │  │ HTTP API      │  │
│  └───────────────┘  │          │  └───────────────┘  │
└─────────────────────┘          └─────────────────────┘
        │                                  │
        │                                  │
        └────────► Network Protocol ◄──────┘
              (PSJM - Custom UDP)
```

### Key Architecture Principles

1. **Server Authority**: Server maintains the single source of truth
2. **Client Prediction**: Clients predict movement for responsiveness
3. **ECS Pattern**: Data-oriented design for performance
4. **Lobby System**: Multiple concurrent game sessions
5. **Event-Driven**: Observer pattern for loose coupling

## 🔗 Documentation Sections

Explore detailed documentation in the following sections:

- **[Key Features](./features.md)**: Complete feature list
- **[Installation Guide](./installation.md)**: Setup instructions
- **[Technologies](./technologies.md)**: Tech stack overview
- **[Architecture](./project_architecure.md)**: Detailed system design
- **[Technical Choices](./project_choices.md)**: Design decisions
- **[Troubleshooting](./troubleshooting.md)**: Common issues and solutions

## 🤝 Contributing

We welcome contributions! Whether you're interested in:
- Adding new game features
- Improving performance optimizations
- Enhancing the documentation
- Fixing bugs or adding tests

Please see our [Contributing Guide](../HOW-TO-CONTRIBUTE/htc-intro.md) for detailed information.

## 📄 Additional Resources

- **[Client Documentation](../CLIENT/client-intro.md)**: Client implementation details
- **[Server Documentation](../SERVER/server-intro.md)**: Server architecture and protocols
- **[ECS Documentation](../ECS/)**: Entity-Component-System framework
- **[Network Protocol](../NETWORK/)**: PSJM protocol specification
- **[Security](../SECURITY/)**: Security considerations

---

**R-Type** represents the culmination of modern software engineering practices applied to game development, providing both an entertaining multiplayer experience and a valuable learning resource for developers interested in high-performance, networked applications.

