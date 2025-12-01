---
sidebar_position: 2
---

# R-Type Project Architecture

This document provides a comprehensive overview of the R-Type project's architecture, including its modular design, component relationships, and design patterns.

## Overview

R-Type is a multiplayer 2D space shooter game built with modern C++20, featuring a client-server architecture with an Entity-Component-System (ECS) design pattern. The project emphasizes modularity, performance, and cross-platform compatibility.


## High-Level Architecture

```mermaid
flowchart TB
    subgraph CLIENT["🎮 Client Application"]
        CC[Client Core]
        CN[Client Network]
        GSM[Game State Machine]
        GFX[Graphics Engine]
        DLL[Dynamic Libraries]
    end

    subgraph SERVER["🖥️ Server Application"]
        SC[Server Core]
        SRV[Game Server]
        CFG[Server Config]
        ECS_S[ECS System]
    end

    subgraph SHARED["🔧 Shared Components"]
        ECS[ECS Framework]
        NET_LIB[Network Library]
        PKT_LIB[Packet Library]
        MUL_LIB[Multimedia Library]
        BUF_LIB[Buffer Library]
    end

    subgraph EXTERNAL["📦 External Dependencies"]
        ASIO[ASIO Networking]
        SFML[SFML Multimedia]
        VCPKG[vcpkg Package Manager]
    end

    CC --> CN
    CC --> GSM
    CC --> GFX
    CC --> DLL

    SC --> SRV
    SC --> CFG
    SC --> ECS_S

    CN --> NET_LIB
    SRV --> NET_LIB
    GFX --> MUL_LIB
    ECS_S --> ECS

    NET_LIB --> BUF_LIB
    PKT_LIB --> BUF_LIB
    MUL_LIB --> SFML
    NET_LIB --> ASIO

    CC <-->|"TCP/UDP"| SC
```

## Core Module Structure

### Client Architecture

```mermaid
flowchart TD
    A[main.cpp] --> B[Core]
    B --> C[ClientNetwork]
    B --> D[GameStateMachine]
    B --> E[ResourceManager]
    B --> F[DLLoader<br/>Window]
    B --> G[DLLoader<br/>Event]

    subgraph "Client Components"
        C
        D
        E
    end

    subgraph "Client Libraries"
        F
        G
    end

    subgraph "Client Features"
        H[Input Handling]
        I[Rendering]
        J[Audio]
        K[Network Sync]
    end

    B --> H
    B --> I
    B --> J
    B --> K
```

### Server Architecture

```mermaid
flowchart TD
    A[main.cpp] --> B[Core]
    B --> C[Server]
    B --> D[ServerConfig]
    B --> E[ResourceManager]

    subgraph "Server Components"
        C
        D
        E
    end

    subgraph "Server Features"
        F[Game Logic]
        G[Physics Simulation]
        H[Player Management]
        I[Network Broadcasting]
        J[State Synchronization]
    end

    B --> F
    B --> G
    B --> H
    B --> I
    B --> J
```

## ECS (Entity-Component-System) Architecture

```mermaid
flowchart TB
    subgraph "Entity Management"
        EM[EntityManager<br/>Creates/Destroys entities]
        RM[ResourceManager<br/>Manages shared resources]
    end

    subgraph "Component Types"
        CP[Position<br/>x, y coordinates]
        CR[Render<br/>Sprite, texture, animation]
        CC[Collision<br/>Hitbox, collision layers]
        CH[Health<br/>HP, damage, status]
        CN[Network<br/>Sync state, ownership]
        CV[Velocity<br/>Speed, direction]
        CA[Animation<br/>Frame sequences]
    end

    subgraph "System Types"
        SR[RenderSystem<br/>Draws entities]
        SP[PhysicsSystem<br/>Movement, collision]
        SN[NetworkSystem<br/>Sync across clients]
        SG[GameLogicSystem<br/>AI, scoring]
        SA[AnimationSystem<br/>Frame updates]
        SD[DamageSystem<br/>Health management]
    end

    subgraph "View System"
        VW[EntityView<br/>Filtered entity access]
        FQ[Query System<br/>Component queries]
    end

    EM --> CP
    EM --> CR
    EM --> CC
    EM --> CH
    EM --> CN
    EM --> CV
    EM --> CA

    SR --> EM
    SP --> EM
    SN --> EM
    SG --> EM
    SA --> EM
    SD --> EM

    VW --> EM
    FQ --> VW

    RM --> EM
```

## Network Architecture

```mermaid
flowchart TD
    subgraph "Client Side"
        CN[ClientNetwork<br/>Connection management]
        CS[State Sync<br/>Receive game state]
        CI[Input Send<br/>Send player input]
    end

    subgraph "Server Side"
        SN[ServerNetwork<br/>Accept connections]
        SS[State Broadcast<br/>Send game state]
        SI[Input Receive<br/>Process player input]
    end

    subgraph "Transport Layer"
        TCP[(TCP<br/>Reliable<br/>Connection setup<br/>Error handling)]
        UDP[(UDP<br/>Fast<br/>Game state<br/>Input packets)]
    end

    subgraph "Packet Types"
        PC[Connection<br/>Packets<br/>Join/Leave game]
        PG[Game State<br/>Packets<br/>Entity updates]
        PI[Input<br/>Packets<br/>Player actions]
        PE[Error<br/>Packets<br/>Disconnects]
    end

    CN --> TCP
    CN --> UDP
    SN --> TCP
    SN --> UDP

    TCP --> PC
    TCP --> PE
    UDP --> PG
    UDP --> PI

    CS --> PG
    CI --> PI
    SS --> PG
    SI --> PI
```

## Library Dependencies

```mermaid
flowchart TD
    subgraph "Core Libraries (/libs)"
        NET[Network Library<br/>TCP/UDP communication]
        MUL[Multimedia Library<br/>Graphics, audio, input]
        PKT[Packet Library<br/>Serialization, protocol]
        BUF[Buffer Library<br/>Memory management]
    end

    subgraph "Network Stack"
        NI[INetwork Interface<br/>Abstract network ops]
        NT[TCP Implementation<br/>Reliable transport]
        NU[UDP Implementation<br/>Fast transport]
    end

    subgraph "Multimedia Stack"
        MI[IWindow Interface<br/>Window management]
        ME[IEvent Interface<br/>Input handling]
        MR[Renderer<br/>Drawing operations]
        MA[Audio System<br/>Sound effects, music]
        MAM[AssetManager<br/>Resource loading]
    end

    subgraph "Packet Stack"
        PB[PacketBase<br/>Packet structure]
        PS[Serialization<br/>Data encoding/decoding]
        PT[PacketTypes<br/>Protocol messages]
    end

    subgraph "Buffer Stack"
        BC[Circular Buffer<br/>Ring buffer]
        BR[Ring Buffer<br/>Memory efficient]
        BM[Memory Management<br/>Allocation strategies]
    end

    NET --> NI
    NI --> NT
    NI --> NU

    MUL --> MI
    MUL --> ME
    MI --> MR
    ME --> MR
    MR --> MA
    MA --> MAM

    PKT --> PB
    PB --> PS
    PS --> PT

    BUF --> BC
    BUF --> BR
    BC --> BM
    BR --> BM

    NET --> BUF
    PKT --> BUF
```

## Data Flow Architecture

```mermaid
sequenceDiagram
    participant Client
    participant ClientNetwork
    participant Server
    participant ServerECS
    participant Database

    Client->>ClientNetwork: Player Input (Key presses, mouse)
    ClientNetwork->>Server: Send Input Packet (UDP)
    Server->>ServerECS: Process Input
    ServerECS->>ServerECS: Update Game State (Physics, AI, collisions)
    ServerECS->>Database: Persist State Changes
    Server->>ClientNetwork: Broadcast Game State (UDP)
    ClientNetwork->>Client: Apply State Updates
    Client->>Client: Render Frame (60 FPS)
    Client->>ClientNetwork: Send Acknowledgment

    Note over Client,Server: Game Loop: 60 FPS<br/>Network: 30-60 updates/sec
```

## Build System Architecture

```mermaid
flowchart LR
    subgraph "Configuration Files"
        CM[CMakeLists.txt<br/>Root build config]
        CP[CMakePresets.json<br/>Build presets]
        VC[vcpkg.json<br/>Dependency manifest]
    end

    subgraph "Build Scripts"
        IS[install_dependencies.sh<br/>Setup environment]
        BS[compile_project.sh<br/>Build all targets]
        CS[clean_project.sh<br/>Clean build artifacts]
        TS[run_unit_tests.sh<br/>Execute tests]
    end

    subgraph "Module Builds"
        CL[client/<br/>Game client]
        SL[server/<br/>Game server]
        COL[common/<br/>Shared code]
        LL[libs/<br/>Core libraries]
        TL[tests/<br/>Unit tests]
    end

    subgraph "Build Targets"
        TC[r-type_client<br/>Client executable]
        TS[r-type_server<br/>Server executable]
        TT[unit_tests<br/>Test runner]
    end

    IS --> VC
    BS --> CM
    CS --> CM
    TS --> TT

    CM --> CL
    CM --> SL
    CM --> COL
    CM --> LL
    CM --> TL

    CL --> TC
    SL --> TS
    TL --> TT
```

## Design Patterns Used

| Pattern | Purpose | Location | Implementation |
|---------|---------|----------|----------------|
| **ECS** | Game object composition and management | `/common/ECS/` | Entity-Component-System architecture |
| **Strategy** | Platform-specific implementations | `/libs/Multimedia/` | Interface-based polymorphism |
| **Observer** | Event handling and notifications | `/common/Signal/` | Signal-slot pattern |
| **Factory** | Entity and prefab creation | `/common/Prefab/` | Centralized object creation |
| **Dynamic Loading** | Runtime library loading | `/common/DLLoader/` | Plugin system for multimedia |
| **Singleton** | Global resource management | Various | Resource managers, configs |
| **Command** | Input handling and replay | Client input system | Input buffering |
| **State** | Game state management | `/client/gsm/` `/server/gsm/` | State machine pattern |

## Directory Structure

```
ryanR-type/
├── client/                    # 🎮 Client application
│   ├── main.cpp              # Application entry point
│   ├── Core.hpp/cpp          # Main client logic
│   ├── ClientNetwork.hpp/cpp # Network communication
│   ├── Utils.hpp/cpp         # Client utilities
│   ├── gsm/                  # Game state machine
│   └── initRessourcesManager/# Resource initialization
├── server/                    # 🖥️ Server application
│   ├── main.cpp              # Server entry point
│   ├── Core.hpp/cpp          # Server core logic
│   ├── Server.hpp/cpp        # Server implementation
│   ├── ServerConfig.hpp/cpp  # Configuration management
│   ├── Utils.hpp/cpp         # Server utilities
│   ├── gsm/                  # Server state machine
│   └── initRessourcesManager/# Server resource init
├── common/                    # 🔧 Shared components
│   ├── ECS/                  # Entity-Component-System
│   │   ├── component/        # Component definitions
│   │   ├── entity/           # Entity management
│   │   ├── system/           # System implementations
│   │   ├── view/             # Entity querying
│   │   └── resourceManager/  # Resource management
│   ├── DLLoader/             # Dynamic library loading
│   ├── Error/                # Error handling system
│   ├── Prefab/               # Object factories
│   ├── Signal/               # Event system
│   ├── constants.hpp         # Global constants
│   └── types/                # Common type definitions
├── libs/                     # 📚 Core libraries
│   ├── Network/              # Network communication
│   ├── Multimedia/           # Graphics, audio, input
│   ├── Packet/               # Data serialization
│   └── Buffer/               # Memory management
├── assets/                   # 🎨 Game assets
│   ├── fonts/                # Font files
│   └── sprites/              # Sprite images
├── tests/                    # 🧪 Testing framework
│   ├── client/               # Client tests
│   ├── server/               # Server tests
│   └── common/               # Shared tests
├── documentation/            # 📖 Project documentation
│   └── docusaurus/           # Docusaurus site
├── scripts/                  # ⚙️ Build and utility scripts
│   ├── compile_project.sh    # Build script
│   ├── install_dependencies.sh # Dependency setup
│   ├── clean_project.sh      # Clean build
│   ├── run_unit_tests.sh     # Test runner
│   └── generate_doc.sh       # Documentation generation
├── CMakeLists.txt            # Root CMake configuration
├── CMakePresets.json         # CMake build presets
├── vcpkg.json               # vcpkg dependency manifest
└── README.md                # Project documentation
```

## Key Architectural Principles

### ✅ **Modularity**
- Clear separation between client, server, and shared components
- Interface-based design for platform abstraction
- Plugin architecture for multimedia backends

### ✅ **Performance**
- ECS architecture for efficient game loop execution
- Data-oriented design for cache-friendly memory access
- Network optimization with TCP/UDP protocol selection

### ✅ **Maintainability**
- Consistent coding standards and comprehensive testing
- Clear dependency management with vcpkg
- Modular build system with CMake

### ✅ **Scalability**
- Plugin-based architecture for extensibility
- Efficient memory management with custom buffer systems
- Network architecture supporting multiple clients

### ✅ **Cross-Platform Compatibility**
- CMake build system for multiple platforms
- Dynamic library loading for platform-specific implementations
- vcpkg for dependency management across platforms

### ✅ **Reliability**
- Comprehensive error handling system
- Unit testing framework
- Network error recovery and state synchronization

## Development Workflow

```mermaid
flowchart TD
    A[Code Changes] --> B[CMake Build]
    B --> C[Unit Tests]
    C --> D[Integration Tests]
    D --> E[Code Review]
    E --> F[Merge to Main]
    F --> G[CI/CD Pipeline]
    G --> H[Deployment]

    B --> I{Tests Pass?}
    I -->|No| J[Fix Issues]
    I -->|Yes| C

    J --> B
```

## Performance Characteristics

- **Target FPS**: 60 FPS for client rendering
- **Network Update Rate**: 30-60 updates per second
- **Latency Target**: < 100ms end-to-end
- **Memory Usage**: Optimized for low-end hardware
- **Platform Support**: Windows, Linux, macOS

## Security Considerations

- **Network Protocol**: Custom binary protocol for efficiency
- **Input Validation**: Server-side validation of all client inputs
- **Resource Limits**: Prevention of resource exhaustion attacks
- **State Synchronization**: Server authoritative game state

This architecture provides a solid foundation for a scalable, maintainable, and performant multiplayer game while following modern software engineering practices.
