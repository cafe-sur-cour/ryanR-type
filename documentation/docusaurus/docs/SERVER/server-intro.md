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
- **Lobby System**: Manages multiple concurrent game instances and player grouping
- **Game State Manager**: Maintains authoritative game state for each lobby
- **Entity Component System (ECS)**: Manages game entities and their behaviors per lobby
- **Packet Manager**: Handles serialization/deserialization of network packets
- **Map System**: Loads and broadcasts map data to clients

### Server Lifecycle

```
Initialization → Listening → Lobby Management → Shutdown
     ↓              ↓           ↓              ↓
   Load Config    Accept Players Create/Join    Cleanup
   Start Network   Assign IDs    Lobbies       Close
   Load Libraries  Process Lobby  Run Game      Connections
                   Requests      Loops
```

## Lobby Management

The server supports multiple concurrent game instances through a lobby system. Each lobby represents a separate game session with its own set of players, game state, and ECS.

### Lobby Creation

- Players can request a lobby code to create a new game instance
- The server generates unique lobby codes for new lobbies
- Each lobby runs in its own thread with dedicated network and game loops

### Lobby Joining

- Players connect to specific lobbies using the lobby code
- The server validates lobby codes and manages player assignment
- Lobbies have configurable maximum player limits

### Lobby Lifecycle

```
Lobby Creation → Player Joining → Game Start → Game Loop → Game End → Cleanup
     ↓                ↓              ↓           ↓           ↓          ↓
  Generate Code    Validate Code   All Ready   Update     End Game   Remove
  Start Threads    Assign Player   Broadcast   Broadcast  Broadcast  Lobby
                   Send Status     CAN_START   State      Winner
```

### Multi-instance (Lobby) — implementation details

The server runs multiple concurrent game instances using a lobby abstraction. Below are the concrete behaviors and implementation notes (source references in `server/`):

- **Lobby creation flow**
  - Client requests a new lobby by sending a `REQUEST_LOBBY` packet (see `PACKET_REQUEST_LOBBY` / `ServerReceivePacket::requestCode`).
  - The server generates a unique alphanumeric lobby code (`Utils::createAlphaNumericCode`) and returns it using `PACKET_SEND_LOBBY_CODE` (`Server::sendCodeLobbyPacket`).
  - A lightweight `LobbyStruct` is pushed to the server's `_lobbyThreads` immediately; when a real lobby instance is required a `Lobby` object is created and added to `_lobbies`. The `Lobby` is initialized with its own `PacketManager` and `ResourceManager`.

- **Joining a lobby**
  - Clients join by sending `CONNECT_TO_LOBBY` with the lobby code. The server verifies the code exists and the lobby isn't full, then replies with `PACKET_LOBBY_CONNECT_VALUE` indicating success/failure (`ServerReceivePacket::processConnectToLobby`).
  - On success the client is added to the `LobbyStruct`'s `_clients` list and the authoritative `Lobby` receives the client via `Lobby::addClient()`; the server maps the client ID to the lobby (`_clientToLobby`) and calls `Lobby::syncExistingEntitiesToClient()` to send current game state.
  - Banned users are rejected during the join flow (server checks `saves/users.json`).

- **Threading model & lifecycle**
  - Each active `Lobby` runs its own threads: a `networkLoop` and a `gameLoop`. Threads are started when the lobby master requests game start (`Server::processMasterStart` → `Lobby::startNetworkThread()` and `Lobby::startGameThread()`).
  - The main `Server` loop handles incoming UDP packets and routes lobby-related packets into the proper lobby by enqueueing packets (see `Server::processIncomingPackets()` and `Lobby::enqueuePacket` / `Lobby::processIncomingPackets`).
  - When a lobby becomes empty the server stops it: `Server` monitors and calls `Lobby::stop()`; resources and threads are cleaned up in `Server::cleanupClosedLobbies()`.

- **Limits & constants**
  - Maximum players per lobby is defined in code as `constants::MAX_CLIENT_PER_LOBBY` (value: 4). The server checks this when accepting joins.
  - There is no explicit hard-coded maximum number of concurrent lobbies — practical limits depend on available CPU/memory and the number of threads created per lobby.

- **Start / stop semantics**
  - Lobby creation returns a lobby code immediately; the authoritative `Lobby` object and its game threads are only started when the lobby master triggers the start sequence.
  - If the last player leaves a lobby, the server will stop the lobby and remove its threads/structures automatically.

- **Operational notes / best practices**
  - The server binds a single UDP port (default `-p 4242`) so running multiple server processes on the same host requires different ports or containerization.
  - For horizontal scaling (many simultaneous lobbies) run multiple server instances behind a load balancer or partition players by region/port; the current architecture keeps all lobbies in a single process and uses per-lobby threads.
  - Monitor `active lobbies`, `players per lobby`, and `tick time per lobby` from the built-in HTTP/management endpoint (`HttpServer`) for operational visibility (`Server::getServerInfo`).

See server sources for authoritative behavior: `server/Server.cpp`, `server/ServerReceivePacket.cpp`, `server/ServerSentPacket.cpp`, `server/Lobby.cpp`, `server/Lobby.hpp`.


## Network Protocol

The server implements the PSJM protocol, handling the following packet types:

### Server-Sent Packets

| Packet Type | Code | Description |
|------------|------|-------------|
| `ACCEPTATION` | 0x02 | Connection acceptance with assigned client ID |
| `END_GAME` | 0x08 | Game end with winner information |
| `CAN_START` | 0x09 | Signal to start player movement |
| `SPAWN_ENTITY` | 0x0A | Entity spawn notification |
| `DEATH_ENTITY` | 0x0B | Entity death notification |
| `SERVER_STATUS` | 0x0C | Lobby status updates |
| `LOBBY_CODE` | 0x0E | Lobby code assignment |
| `LOBBY_CONNECT_VALUE` | 0x11 | Lobby join confirmation |

### Client-Received Packets

| Packet Type | Code | Description |
|------------|------|-------------|
| `CONNECTION_CLIENT` | 0x01 | Player connection request |
| `DISCONNECTION` | 0x03 | Player disconnection |
| `EVENT` | 0x04 | Player input events |
| `CLIENT_READY` | 0x08 | Player readiness signal |
| `REQUEST_LOBBY` | 0x0D | Lobby creation request |
| `CONNECT_TO_LOBBY` | 0x0F | Lobby join request |
| `LOBBY_MASTER_START` | 0x10 | Game start request from lobby master |

## Configuration

The server can be configured through `ServerConfig`:

```cpp
struct ServerConfig {
    uint16_t port;           // Server port (default: 4242)
    std::string ip;          // IP address to bind to (default: 127.0.0.1)
    int64_t tps;             // Ticks per second (default: 20)
    bool isDebug;            // Debug mode flag (default: false)
};
```

Note: Maximum concurrent players is fixed at 4 (MAX_CLIENT constant).

## Game State Management

Each lobby maintains its own authoritative game state, ensuring isolation between concurrent games.

### Authority Model

The server maintains the **single source of truth** for each lobby:
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
4. Client requests lobby creation/join (REQUEST_LOBBY/CONNECT_TO_LOBBY)
5. Server validates lobby code and assigns client to lobby
6. Server sends LOBBY_CONNECT_VALUE (0x11) confirmation
7. Client signals readiness with CLIENT_READY (0x08)
8. Server broadcasts SERVER_STATUS (0x0C) with lobby info
9. When all players ready, server broadcasts CAN_START (0x09)
10. Game loop begins with synchronized start
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
- **Multithreading**: Separate network I/O from game logic, with per-lobby threads

### Monitoring

Key metrics tracked:
- Active lobbies
- Players connected per lobby
- Total players connected
- Packets sent/received per second
- Average tick time per lobby
- Packet loss rate

## Usage

### Starting the Server

```bash
# Default configuration
./r-type_server

# Custom port and IP
./r-type_server -p 8080 -i 0.0.0.0

# With custom TPS and debug mode
./r-type_server -p 4242 -i 127.0.0.1 -tps 30 -d
```

### Command Line Options

```
Usage: ./r-type_server [options]
Options:
  -p <port>        Specify the port number (default: 4242) (5173 is reserved for HTTP server)
  -i <ip_address>  Specify the IP address to bind to (default: 127.0.0.1)
  -tps <tps>       Specify the TPS (ticks per second) (default: 20)
  -d               Enable debug mode
  -h               Display this help message

Example:
  ./r-type_server -p 4243 -i 127.0.0.1
```

Note: The maximum number of clients is fixed at 4 (MAX_CLIENT constant).

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
[INFO] New lobby created with code: ABC123
[INFO] Client 42 joined lobby ABC123
[DEBUG] Received EVENT packet from client 42 in lobby ABC123
[INFO] Game state updated (lobby: ABC123, tick: 1234, players: 3)
[WARN] Client 12 timeout in lobby ABC123, disconnecting
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
