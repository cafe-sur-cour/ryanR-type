---
sidebar_position: 1
---

# R-Type Network

This section covers the network architecture and implementations for R-Type, focusing on real-time multiplayer communication.

## Overview

R-Type requires efficient, low-latency network communication for multiplayer gameplay. The networking layer is built on **UDP** with custom reliability mechanisms to ensure critical game data delivery while maintaining the performance necessary for real-time action.

## Architecture

### Protocol Stack

```
Application Layer    → PSJM Protocol (Multiplayer Game Sync)
Transport Layer      → UDP (Low latency, connectionless)
Serialization        → Custom Binary Protocol (Packet Manager)
Network Library      → ASIO (Asynchronous I/O)
```

### Key Design Decisions

| Aspect | Choice | Rationale |
|--------|--------|-----------|
| **Transport** | UDP | Low latency for real-time gameplay |
| **Reliability** | Selective | Critical packets only (connection, map) |
| **State Sync** | Server Authority | Prevent cheating, consistent state |
| **Update Rate** | 20 FPS | Balance between responsiveness and bandwidth |
| **Packet Format** | Binary | Compact size, fast serialization |

## Network Flow

### Connection Establishment

```
Client                          Server
  |                               |
  |---(1) CONNECTION_CLIENT------>|
  |                               |
  |<--(2) ACCEPTATION-------------|
  |                               |
  |<--(3) MAP_SEND----------------|
  |<--(4) MAP_SEND (continued)----|
  |<--(5) END_MAP-----------------|
  |                               |
  |<--(6) CAN_START---------------|
  |                               |
  |===== Game Loop Begins ========|
```

### Game Loop Communication

```
Client                          Server
  |                               |
  |---(EVENT: Move Right)-------->|
  |                               |
  |<--(GAME_STATE: All entities)--|
  |                               |
  |---(EVENT: Shoot)------------->|
  |                               |
  |<--(GAME_STATE: Updated)-------|
  |                               |
```

## Packet Structure

All packets follow a standardized format defined in the **Packet Manager** library.

### Packet (11 bytes + (Length bytes))

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| Magic (0x93)  |  Client ID    |        Sequence Number        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     Type      |             Payload Length                    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|      Length (cont.)           |             Body (...)        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

## Network Protocols

### UDP - User Datagram Protocol

**Advantages:**
- ✅ Low latency (no handshake overhead)
- ✅ No head-of-line blocking
- ✅ Suitable for real-time applications
- ✅ Simple, lightweight

**Challenges:**
- ❌ No guaranteed delivery
- ❌ Packets can arrive out of order
- ❌ No congestion control
- ❌ Potential for packet loss

**Our Solution:**
- Use sequence numbers to detect loss
- Implement selective reliability for critical packets
- Client-side prediction for smooth gameplay
- Server as authoritative state

### Reliability Mechanisms

#### 1. Sequence Numbers

Every packet includes a sequence number to:
- Detect missing packets
- Identify out-of-order delivery
- Track packet loss rate

```cpp
packet.setSequenceNumber(currentSeq++);
```

#### 2. Client Prediction

Clients predict entity positions between server updates:

```cpp
// Predict position based on velocity
predictedPosition = lastPosition + velocity * timeDelta;

// Correct when server update arrives
if (serverUpdate.position != predictedPosition) {
    interpolate(predictedPosition, serverUpdate.position);
}
```

#### 3. Server Authority

The server maintains the **authoritative game state**:
- All game logic executes on server
- Clients send inputs, not positions
- Server validates and broadcasts results

## Key Concepts

### TCP vs UDP

| Feature | TCP | UDP |
|---------|-----|-----|
| Connection | Connection-oriented | Connectionless |
| Reliability | Guaranteed delivery | Best effort |
| Ordering | Maintains order | No ordering |
| Speed | Slower (overhead) | Faster |
| Use Case | File transfer, web | Real-time games, streaming |

**Why UDP for R-Type:**
- **Latency Sensitive**: Player actions must be reflected immediately
- **State Overwrite**: Newer game states supersede old ones
- **Bandwidth**: Binary protocol minimizes packet size
- **Custom Control**: We implement only necessary reliability

### Handshake Mechanism

For critical operations (connection, map loading), we implement a simple handshake over UDP:

```cpp
// Connection handshake
Client → Server: CONNECTION_CLIENT (0x01)
Server → Client: ACCEPTATION (0x02)

// If no response after timeout, retry
if (!receivedAcceptation && timeout > 3s) {
    resendConnection();
}
```

### State Synchronization

#### Server Broadcast (20 FPS)

Every 50ms, the server sends a `GAME_STATE` packet containing:
- All player positions and velocities
- Enemy states
- Projectile locations
- Collectible items
- Score updates

#### Delta Encoding

To optimize bandwidth, only changed entities are transmitted:

```cpp
if (entity.hasChanged()) {
    gameState.addEntity(entity);
}
```

## Network Performance

### Bandwidth Optimization

**Techniques Used:**
- Binary serialization (vs JSON/XML)
- Bit packing for boolean values
- Fixed-point numbers (vs floating-point)
- Delta compression

**Example Packet Sizes:**
- Header: 11 bytes
- EVENT packet: 3 bytes (total: 14 bytes)
- GAME_STATE: Variable (avg: 100-300 bytes)

### Latency Considerations

**Target Metrics:**
- Server tick rate: 20 FPS (50ms)
- Acceptable RTT: < 100ms
- Client prediction window: 100ms
- Interpolation buffer: 50ms

### Packet Loss Handling

**Strategy:**
1. **Non-critical packets** (GAME_STATE): Drop old, use latest
2. **Critical packets** (CONNECTION, MAP): Retry with exponential backoff
3. **High loss rate** (> 10%): Warn player of network issues

## Network Security

### Validation

All incoming packets are validated:

```cpp
// Check magic number
if (packet.getMagicNumber() != 0x93) {
    reject();
}

// Verify client ID
if (!isValidClient(packet.getClientId())) {
    reject();
}

// Validate packet structure
if (!packet.hasValidEndMarker()) {
    reject();
}
```

### Rate Limiting

Prevent spam attacks:

```cpp
const int MAX_PACKETS_PER_SECOND = 60;
if (clientPacketCount > MAX_PACKETS_PER_SECOND) {
    dropPacket();
}
```

### Client Authority Limits

Clients can only:
- Send their own inputs
- Request disconnection
- Receive game state

Clients **cannot**:
- Modify other players' states
- Spawn entities directly
- Change game rules

## Implementation

### Client-Side

```cpp
// Send player input
void sendEvent(EventType event) {
    PacketManager packet(sequenceNumber++);
    packet.setIdClient(clientId);
    packet.setType(EVENT_PACKET);

    std::vector<uint64_t> payload = {
        EVENT_PACKET,
        static_cast<uint64_t>(event)
    };

    auto data = packet.pack(payload);
    socket.send(serverEndpoint, data);
}

// Receive game state
void receiveGameState() {
    auto data = socket.receive();
    PacketManager packet(0);

    if (packet.unpack(data)) {
        updateGameEntities(packet.getPayload());
    }
}
```

### Server-Side

```cpp
// Broadcast game state
void broadcastGameState() {
    PacketManager packet(sequenceNumber++);
    packet.setType(GAME_STATE_PACKET);

    auto payload = serializeGameState();
    auto data = packet.pack(payload);

    for (auto& client : connectedClients) {
        socket.send(client.endpoint, data);
    }
}

// Handle client event
void handleEvent(PacketManager& packet) {
    uint8_t clientId = packet.getIdClient();
    auto payload = packet.getPayload();

    EventType event = static_cast<EventType>(payload[1]);
    processPlayerInput(clientId, event);
}
```

## Debugging Network Issues

### Common Problems

**1. High Latency**
```bash
# Check ping to server
ping <server_ip>

# Analyze packet timing
tcpdump -i any -tttt port 4242
```

**2. Packet Loss**
```bash
# Monitor packet loss
netstat -s | grep -i "packet.*loss"

# Use mtr for continuous monitoring
mtr <server_ip>
```

**3. Firewall Issues**
```bash
# Check if UDP port is accessible
nc -u -v <server_ip> 4242

# Allow port in firewall (Linux)
sudo ufw allow 4242/udp
```

### Network Analysis Tools

**Wireshark:**
```
Filter: udp.port == 4242
Look for: Packet types, sequence gaps, timing
```

**tcpdump:**
```bash
# Capture R-Type packets
sudo tcpdump -i any -X 'udp port 4242'

# Save to file for analysis
sudo tcpdump -i any -w rtype.pcap 'udp port 4242'
```

## Proof of Concepts

For detailed analysis and comparison of network approaches, see our [Network POCs](../POCS/network/pocs-network.md).

## Related Documentation

- [Packet Management](./packet-management.md) - Detailed packet format and serialization
- [Server Architecture](../SERVER/server-intro.md) - Server implementation details
- [RFC Protocol](../SERVER/rfc.md) - Complete protocol specification
- [Network POCs](../POCS/network/pocs-network.md) - Protocol comparisons and benchmarks

## References

- [UDP - User Datagram Protocol (RFC 768)](https://tools.ietf.org/html/rfc768)
- [Game Networking Patterns](https://gafferongames.com/)
- [Valve's Networking in Source Engine](https://developer.valvesoftware.com/wiki/Source_Multiplayer_Networking)