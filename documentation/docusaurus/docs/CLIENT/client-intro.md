---
sidebar_position: 1
---

# R-Type — Client Overview

This page provides a comprehensive introduction to the R‑Type client component: what it contains, how to build and run it for development, and where to look for common features (input, rendering, audio, UI).

## Purpose

The client implements the graphical and audio front-end for the game:
- **Rendering**: Visual display using SFML
- **Input handling**: Keyboard, mouse, and gamepad support
- **Local state interpolation**: Smooth gameplay despite network latency
- **UI management**: Menus, HUD, and widgets
- **Audio playback**: Music and sound effects

It connects to the server for multiplayer state synchronization and uses the project's common libraries for ECS, resource management, and network packets.

## Quick Links

- **[User Guide](./user-guide.md)**: How to play the game
- **[Client Architecture](./client-architecture.md)**: Technical implementation details
- **[Installation](../INTRO/installation.md)**: Setup instructions

## Key Features

### Graphics
- **SFML-based rendering**: Hardware-accelerated 2D graphics
- **Sprite animations**: Frame-based animation system
- **Parallax backgrounds**: Multi-layer scrolling backgrounds
- **Particle effects**: Explosions and visual effects
- **Layer management**: Z-ordering for proper rendering

### Input
- **Multiple input devices**: Keyboard, mouse, gamepad
- **Remappable controls**: Full key/button remapping
- **Input mapping system**: Action-based input abstraction
- **Gamepad support**: Xbox, PlayStation, and generic controllers

### Audio
- **Background music**: OGG Vorbis streaming
- **Sound effects**: WAV format with pooling
- **Positional audio**: 3D sound positioning
- **Volume controls**: Separate music/SFX volume

### Networking
- **UDP communication**: Low-latency multiplayer
- **Client prediction**: Local movement prediction
- **Entity interpolation**: Smooth remote entity movement
- **Packet handling**: Custom binary protocol

### Accessibility
- **Color-blind modes**: Deuteranopia, Protanopia, Tritanopia shaders
- **High contrast mode**: Enhanced visibility
- **UI scaling**: Adjustable interface size
- **Alternative indicators**: Numeric health, audio cues

## Important Client Folders and Files

### Core Files

```
client/
├── main.cpp                    # Application entry point
├── Core.cpp / Core.hpp         # Main game loop and lifecycle
├── ClientNetwork.cpp/hpp       # Network communication
├── Utils.cpp/hpp               # CLI argument parsing
├── constants.hpp               # Client-side constants
└── SettingsManager.cpp/hpp     # Persistent settings
```

### Game State Management

```
client/gsm/
├── GameStateMachine.cpp        # State management
└── states/
    ├── MenuState.cpp           # Main menu
    ├── LobbyState.cpp          # Lobby creation/joining
    ├── InGameState.cpp         # Active gameplay
    └── PauseState.cpp          # Pause overlay
```

### Systems

```
client/systems/
├── rendering/
│   ├── RenderSystem.cpp        # Main rendering
│   ├── ParallaxSystem.cpp      # Background scrolling
│   └── AnimationSystem.cpp     # Sprite animations
├── audio/
│   ├── MusicSystem.cpp         # Background music
│   └── SoundSystem.cpp         # Sound effects
└── input/
    └── InputSystem.cpp         # Input processing
```

### Components

```
client/components/
├── SpriteComponent.hpp         # Visual representation
├── AnimationComponent.hpp      # Animation data
├── CameraComponent.hpp         # Viewport control
└── UIComponent.hpp             # UI elements
```

### UI System

```
client/ui/
├── UIManager.cpp               # UI management
├── Widget.cpp                  # Base widget class
└── widgets/
    ├── Button.cpp              # Clickable buttons
    ├── TextBox.cpp             # Text input/display
    ├── Slider.cpp              # Value sliders
    └── Label.cpp               # Text labels
```

### Resource Management

```
client/initResourcesManager/
└── GraphicalInputProvider.cpp  # Resource initialization
```

### Network

```
client/packet/
├── ClientSentPacket.cpp        # Outgoing packets
└── ClientReceivedPacket.cpp    # Incoming packet handling
```

## Shared Resources

### Assets

```
assets/
├── fonts/                      # TrueType fonts for UI
├── musics/                     # Background music (OGG)
├── shaders/                    # GLSL shaders
│   ├── deuteranopia.frag       # Color-blind shader
│   ├── protanopia.frag
│   └── tritanopia.frag
├── sounds/                     # Sound effects (WAV)
├── sprites/                    # Game sprites (PNG)
└── ui/                         # UI textures
```

### Configuration

```
saves/
├── settings.json               # Graphics, audio, network settings
├── keybinds.json              # Custom key mappings
├── accessibility.json          # Accessibility options
├── scores.json                # High scores
└── users.json                 # Player profiles
```

### Common Libraries

```
common/
├── ECS/                       # Entity-Component-System
├── components/                # Shared game components
├── systems/                   # Shared game systems
├── packet/                    # Network packet definitions
└── InputMapping/              # Input abstraction
    ├── InputMapping.hpp
    ├── InputMappingManager.hpp
    └── InputAction.hpp
```

## Building the Client

### Prerequisites

- **C++ Compiler**: GCC 11+, Clang 12+, or MSVC 2019+
- **CMake**: 3.23+
- **vcpkg**: For dependency management
- **SFML**: 2.6+ (auto-installed via vcpkg)

### Compilation

**Unix (Linux/macOS):**
```bash
./scripts/install_dependencies.sh
./scripts/compile_project.sh

# Client binary
./r-type_client
```

**Windows:**
```cmd
.\scripts\install_dependencies.bat
.\scripts\compile_project.bat

REM Client binary
.\r-type_client.exe
```

### CMake Configuration

```bash
# Configure
cmake --preset unix-release

# Build
cmake --build build/unix-release --target r-type_client

# Run
./build/unix-release/r-type_client
```

## Running the Client

### Basic Usage

```bash
# Connect to localhost with default name
./r-type_client

# Specify server and player name
./r-type_client -i 192.168.1.100 -p 4242 -n Alice

# Enable debug logging
./r-type_client -d
```

### Command-Line Arguments

| Argument | Description | Default |
|----------|-------------|---------|
| `-i <ip>` | Server IP address | 127.0.0.1 |
| `-p <port>` | Server port | 4242 |
| `-n <name>` | Player name | Auto-generated |
| `-d` | Enable debug mode | Disabled |
| `-h` | Display help | - |

## Key Features and Where to Find Them

### Input Remapping

**Location:** `common/InputMapping/`

**Files:**
- `InputMapping.hpp`: Individual key-action binding
- `InputMappingManager.hpp`: Manages all bindings
- `InputAction.hpp`: Action definitions

**Usage:**
```cpp
// Define actions
enum class InputAction {
    MoveUp, MoveDown, MoveLeft, MoveRight,
    Shoot, Special, Pause
};

// Load mappings
InputMappingManager mappings;
mappings.load("saves/keybinds.json");

// Check input
if (mappings.isActionPressed(InputAction::Shoot)) {
    // Fire weapon
}
```

### Rendering Systems

**Location:** `client/systems/rendering/`

**Files:**
- `RenderSystem.cpp`: Main sprite rendering
- `ParallaxSystem.cpp`: Multi-layer backgrounds
- `AnimationSystem.cpp`: Frame-based animations
- `HitboxRenderSystem.cpp`: Debug collision visualization

**Rendering Order:**
1. Parallax background (far → near)
2. Game entities (sorted by layer)
3. Particle effects
4. UI / HUD
5. Debug overlays (if enabled)

### Audio Systems

**Location:** `client/systems/audio/`

**Files:**
- `MusicSystem.cpp`: Background music management
- `SoundSystem.cpp`: Sound effect playback

**Features:**
- Music streaming (OGG Vorbis)
- Sound pooling for effects
- Positional 3D audio
- Volume mixing (master, music, SFX)

### Network Packet Handling

**Location:** `client/packet/` and `common/packet/`

**Client Sent Packets:**
- `CONNECTION_CLIENT`: Initial connection request
- `EVENT`: Player input events
- `CLIENT_READY`: Ready status
- `REQUEST_LOBBY`: Create lobby
- `CONNECT_TO_LOBBY`: Join lobby
- `DISCONNECTION`: Clean disconnect

**Client Received Packets:**
- `ACCEPTATION`: Connection accepted with client ID
- `SPAWN_ENTITY`: New entity in game
- `DEATH_ENTITY`: Entity destruction
- `CAN_START`: Game start signal
- `END_GAME`: Game over with results
- `SERVER_STATUS`: Lobby information
- `LOBBY_CODE`: Lobby code assignment

### UI Components

**Location:** `client/ui/`

**Widget Hierarchy:**
```
Widget (base class)
├── Button
├── TextBox
├── Slider
├── Label
├── Image
└── Container
    ├── Panel
    └── ScrollPanel
```

**Example Usage:**
```cpp
// Create a button
auto button = std::make_shared<Button>();
button->setText("Start Game");
button->setPosition(100, 200);
button->setOnClick([]() {
    // Start game logic
});

uiManager->addWidget(button);
```

## Client Architecture Patterns

### Game State Machine

States manage different screens:
- **MenuState**: Main menu, navigation
- **LobbyState**: Multiplayer lobby
- **InGameState**: Active gameplay
- **PauseState**: Pause overlay

**State Lifecycle:**
```cpp
enter()  → update() → render() → exit()
   ↓         ↓          ↓         ↓
 Setup    Game Loop   Draw    Cleanup
```

### Client Prediction

To compensate for network latency:

1. **Predict**: Apply input immediately on client
2. **Send**: Transmit input to server
3. **Receive**: Get authoritative state from server
4. **Reconcile**: Correct if prediction was wrong

```cpp
// Prediction
localPlayer.x += input.dx;

// Reconciliation
if (abs(localPlayer.x - serverState.x) > threshold) {
    localPlayer.x = serverState.x;  // Correct
}
```

### Entity Interpolation

Remote entities are interpolated between server updates for smooth movement:

```cpp
// Interpolate from previous to current server state
entity.x = lerp(prevState.x, currentState.x, alpha);
```

## Performance Considerations

### Frame Rate

- **Target**: 60 FPS
- **VSync**: Optional (can be toggled)
- **Frame limiting**: Configurable in settings

### Memory Management

- **Resource caching**: Textures/sounds loaded once
- **Entity pooling**: Reuse bullet/effect entities
- **Component cleanup**: Automatic via shared_ptr

### Network Optimization

- **Packet batching**: Multiple inputs per packet
- **Delta compression**: Only send changes
- **Client prediction**: Reduce perceived latency

## Development Workflow

### Adding a New System

1. Create system class inheriting from `ASystem`
2. Implement `update(float deltaTime)` method
3. Register system in game state
4. Add to update loop

```cpp
class MySystem : public ASystem {
public:
    void update(float deltaTime) override {
        auto view = registry->view<MyComponent>();
        for (Entity e : view) {
            // Process entities
        }
    }
};

// In InGameState
systems.push_back(std::make_shared<MySystem>());
```

### Adding a New UI Widget

1. Inherit from `Widget` base class
2. Implement render and event handling
3. Add to `UIManager`

```cpp
class MyWidget : public Widget {
public:
    void render(sf::RenderWindow& window) override {
        // Draw widget
    }
    
    void handleEvent(const sf::Event& event) override {
        // Process input
    }
};
```

## Testing

### Running Tests

```bash
# Build with tests
cmake --preset unix-release -DBUILD_TESTS=ON
cmake --build build/unix-release

# Run tests
ctest --test-dir build/unix-release
```

### Debug Tools

**Debug Overlay (F3):**
- FPS counter
- Entity count
- Network ping
- Memory usage

**Debug Rendering:**
- Collision hitboxes
- Entity IDs
- Velocity vectors

## Common Issues

### Graphics Not Displaying

- Verify SFML is properly installed
- Check assets folder exists
- Update graphics drivers
- Run from project root directory

### Audio Not Working

- Verify audio files exist
- Check system volume
- Update audio drivers
- Test with different audio backend

### Input Not Responding

- Check keybindings in `saves/keybinds.json`
- Verify input device is connected
- Test with different input device
- Reset keybinds to default

### Network Connection Failed

- Verify server is running
- Check firewall settings
- Use correct IP and port
- Test with localhost first

## Additional Resources

- **[User Guide](./user-guide.md)**: Complete gameplay guide
- **[Client Architecture](./client-architecture.md)**: Detailed technical documentation
- **[Troubleshooting](../INTRO/troubleshooting.md)**: Common problems and solutions
- **[Network Protocol](../NETWORK/network-intro.md)**: Protocol specification
- **[ECS Documentation](../ECS/ecs-intro.md)**: Entity-Component-System details

---

The client provides a rich, responsive gaming experience with support for various input devices, accessibility features, and smooth multiplayer gameplay.
