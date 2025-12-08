---
sidebar_position: 1
---

# R-Type Server

## Overview

The R-Type server is the authoritative component responsible for managing game state, player connections, and multiplayer synchronization. Built on the **Multiplayer Game Synchronization Protocol (PSJM)**, it ensures consistent gameplay across all connected clients.

## Architecture

### Core Components

The server is structured around several key modules:

- **Network Layer**: UDP-based communication using ASIO
- **Game State Manager**: Maintains authoritative game state
- **Entity Component System (ECS)**: Manages game entities and their behaviors
- **Packet Manager**: Handles serialization/deserialization of network packets
- **Map System**: Loads and broadcasts map data to clients

### Server Lifecycle

```
Initialization → Listening → Game Loop → Shutdown
     ↓              ↓           ↓           ↓
   Load Map    Accept Players  Update    Cleanup
   Load Config   Send Map     Broadcast  Close
   Start ECS    Synchronize   Process   Connections
```

## Network Protocol

The server implements the PSJM protocol, handling the following packet types:

### Server-Sent Packets

| Packet Type | Code | Description |
|------------|------|-------------|
| `ACCEPTATION` | 0x02 | Connection acceptance with assigned client ID |
| `GAME_STATE` | 0x05 | Game state updates (20 FPS) |
| `MAP_SEND` | 0x06 | Map data transmission |
| `END_MAP` | 0x07 | Map completion notification |
| `END_GAME` | 0x08 | Game end with winner information |
| `CAN_START` | 0x09 | Signal to start player movement |

### Client-Received Packets

| Packet Type | Code | Description |
|------------|------|-------------|
| `CONNECTION_CLIENT` | 0x01 | Player connection request |
| `DISCONNECTION` | 0x03 | Player disconnection |
| `EVENT` | 0x04 | Player input events |

## Configuration

The server can be configured through `ServerConfig`:

```cpp
struct ServerConfig {
    uint16_t port;           // Server port (default: 4242)
    size_t maxPlayers;       // Maximum concurrent players
    float tickRate;          // Updates per second (default: 20)
    std::string mapPath;     // Path to map configuration
};
```

## Game State Management

### Authority Model

The server maintains the **single source of truth** for:
- Player positions and velocities
- Enemy spawns and movements
- Projectile trajectories
- Collision detection
- Score and health

### Update Cycle

1. **Receive Events**: Process all client input packets
2. **Update ECS**: Run all game systems (movement, collision, AI)
3. **Broadcast State**: Send game state to all clients
4. **Handle Disconnections**: Clean up disconnected players

### Synchronization

- **Update Rate**: 20 FPS (50ms per tick)
- **Interpolation**: Clients predict between updates
- **Rollback**: Server state is authoritative

## Player Connection Flow

```cpp
1. Client sends CONNECTION_CLIENT (0x01) with player name
2. Server validates and assigns unique ID (1-255)
3. Server responds with ACCEPTATION (0x02) containing ID
4. Server sends MAP_SEND (0x06) with map data
5. Server broadcasts CAN_START (0x09) when ready
6. Game loop begins
```

## Map System

### Map Loading

Maps are loaded from JSON configuration files:

```json
{
  "map_id": "level_01",
  "duration": 120,
  "enemies": [
    {
      "type": "basic_enemy",
      "spawn_time": 5.0,
      "position": {"x": 800, "y": 300}
    }
  ],
  "obstacles": [...],
  "background": "assets/backgrounds/space.png"
}
```

### Map Broadcasting

The server sends map data to clients in chunks to avoid UDP packet size limits:
- Each `MAP_SEND` packet contains a portion of the map
- Sequence numbers track packet order
- `END_MAP` signals complete transmission

## Error Handling

### Packet Loss

- **Sequence Numbers**: Track packet order
- **Client Prediction**: Interpolate missing updates
- **Retry Logic**: Critical packets (connection, disconnection) are acknowledged

### Connection Issues

- **Timeout Detection**: 5 seconds without heartbeat
- **Graceful Disconnection**: Clean up player entity
- **Broadcast**: Notify other clients of disconnection

### Validation

All incoming packets are validated for:
- Magic number (0x93)
- Valid client ID
- Correct packet structure

## Performance

### Optimization Strategies

- **Packet Pooling**: Reuse packet objects
- **Delta Compression**: Only send changed state
- **Spatial Partitioning**: Reduce collision checks
- **Multithreading**: Separate network I/O from game logic

### Monitoring

Key metrics tracked:
- Players connected
- Packets sent/received per second
- Average tick time
- Packet loss rate

## Usage

### Starting the Server

```bash
# Default configuration
./r-type_server

# Custom port and map
./r-type_server --port 8080 --map configs/map/level_02.json

# Verbose logging
./r-type_server --verbose
```

### Command Line Options

```
Options:
  --port PORT        Server port (default: 4242)
  --max-players N    Maximum players (default: 4)
  --map PATH         Map configuration file
  --tick-rate HZ     Update frequency (default: 20)
  --verbose          Enable debug logging
```

## Security Considerations

- **Rate Limiting**: Prevent spam from malicious clients
- **Input Validation**: Sanitize all client data
- **Client ID Verification**: Ensure clients only control their entities
- **Timeout Protection**: Auto-disconnect inactive clients

## Debugging

### Logging

The server provides detailed logs:

```
[INFO] Server started on port 4242
[INFO] Client 42 connected from 192.168.1.10:54321
[DEBUG] Received EVENT packet from client 42
[INFO] Game state updated (tick: 1234, players: 3)
[WARN] Client 12 timeout, disconnecting
[ERROR] Failed to parse packet: Invalid magic number
```

### Network Analysis

Use `tcpdump` or Wireshark to analyze packets:

```bash
sudo tcpdump -i any -X port 4242
```

## See Also

- [RFC Protocol Specification](./rfc.md)
- [Packet Management](../NETWORK/packet-management.md)
- [Network POCs](../POCS/network/pocs-network.md)
