---
sidebar_position: 1
---

# Packet Management

## Overview

The packet management system is a shared library that provides a standardized interface for serialization, deserialization, and manipulation of network packets in the R-Type project. This library ensures reliable and structured communication between the client and server.

## Architecture

### Shared Library

The Packet Manager is implemented as a **dynamic shared library** (`.so` on Linux, `.dylib` on macOS, `.dll` on Windows). This approach offers several advantages:

- **Modularity**: Packet management code can be reused by different components
- **Maintenance**: Library updates don't require recompiling the entire project
- **Independence**: Client and server share the same packet logic
- **Dynamic Loading**: Ability to load the library at runtime via `dlopen`/`LoadLibrary`

```cmake
add_library(Packet SHARED
    IPacketManager.hpp
    PacketManager.hpp
    PacketManager.cpp
    Pack.cpp
    Unpack.cpp
    PacketTypes.cpp
)
```

The library is automatically copied to the `librairies/` folder after compilation to facilitate deployment.

## Packet Structure

### General Format

Each packet consists of two main parts:

1. **Header** - 13 bytes
2. **Body** - Variable size depending on packet type

### Header Format

```
+---------------+---------------+---------------+---------------+
|  Magic Number |   Client ID   |      Sequence Number (4 bytes) |
|   (1 byte)    |   (1 byte)    |                                |
+---------------+---------------+---------------+---------------+
|     Type      |        Length (4 bytes)       |   End Marker  |
|   (1 byte)    |                               |   (2 bytes)   |
+---------------+---------------+---------------+---------------+
```

| Field | Size | Description |
|-------|------|-------------|
| Magic Number | 1 byte | Unique protocol identifier (0x93) |
| Client ID | 1 byte | Client identifier (0-255) |
| Sequence Number | 4 bytes | Packet sequence number |
| Type | 1 byte | Packet type (see Packet Types) |
| Length | 4 bytes | Total body length |
| End Marker | 2 bytes | End marker (`\r\n`) |

### Body Format

The body format varies depending on the packet type. Each body ends with the `\r\n` marker (2 bytes).

## Interface IPacketManager

```cpp
namespace pm {
class IPacketManager {
    public:
        // Getters
        virtual uint8_t getMagicNumber() const = 0;
        virtual uint32_t getLength() const = 0;
        virtual uint32_t getSequenceNumber() const = 0;
        virtual uint8_t getType() const = 0;
        virtual std::vector<uint64_t> getPayload() const = 0;
        virtual uint8_t getIdClient() const = 0;

        // Setters
        virtual void setType(uint8_t type) = 0;
        virtual void setLength(uint32_t length) = 0;
        virtual void setSequenceNumber(uint32_t sequenceNumber) = 0;
        virtual void setPayload(std::vector<uint64_t> payload) = 0;
        virtual void setIdClient(uint8_t idClient) = 0;

        // Pack/Unpack
        virtual std::vector<uint8_t> pack(uint8_t idClient,
                                          uint32_t sequenceNumber,
                                          uint8_t type) = 0;
        virtual std::vector<uint8_t> pack(std::vector<uint64_t> payload) = 0;
        virtual bool unpack(std::vector<uint8_t> data) = 0;

        // Utilitaires
        virtual void reset() = 0;
};
}
```

## Packet Types

| Type | Code | Description | Body Size |
|------|------|-------------|-----------|
| NO_OP | 0x00 | Empty/test packet | Variable |
| CONNECTION_CLIENT | 0x01 | Client connection request | 11 bytes |
| ACCEPTATION | 0x02 | Connection acceptance | 4 bytes |
| DISCONNECTION | 0x03 | Disconnection | 4 bytes |
| EVENT | 0x04 | Game event | 5 bytes |
| GAME_STATE | 0x05 | Game state | Variable |
| MAP_SEND | 0x06 | Map sending | Variable |
| END_MAP | 0x07 | Map end | Variable |
| END_GAME | 0x08 | Game end | Variable |
| CAN_START | 0x09 | Start authorization | Variable |

## Serialization System

### Endianness Support

The library supports two data encoding formats:

#### Big-Endian (Network Byte Order)
```cpp
pm::BigEndianSerialization serializer;
// 0x12345678 → [0x12, 0x34, 0x56, 0x78]
```

#### Little-Endian (Intel x86/x64)
```cpp
pm::LittleEndianSerialization serializer;
// 0x12345678 → [0x78, 0x56, 0x34, 0x12]
```

### Interface ISerializer

```cpp
class ISerializer {
    public:
        // Serialization
        virtual std::vector<uint8_t> serializeUInt(uint64_t value) = 0;
        virtual std::vector<uint8_t> serializeULong(uint64_t value) = 0;
        virtual std::vector<uint8_t> serializeUShort(uint64_t value) = 0;
        virtual std::vector<uint8_t> serializeUChar(uint64_t value) = 0;

        // Deserialization
        virtual uint64_t deserializeUInt(std::vector<uint8_t> data) = 0;
        virtual uint64_t deserializeULong(std::vector<uint8_t> data) = 0;
        virtual uint64_t deserializeUShort(std::vector<uint8_t> data) = 0;
        virtual uint64_t deserializeUChar(std::vector<uint8_t> data) = 0;
};
```

## Usage

### Creating a Packet

```cpp
#include "PacketManager.hpp"

// Create a packet manager with an initial sequence number
pm::PacketManager packet(1);

// Configure the packet
packet.setIdClient(42);
packet.setType(CONNECTION_CLIENT_PACKET);
```

### Packing a Header

```cpp
// Create a header
uint8_t clientId = 10;
uint32_t sequenceNumber = 1;
uint8_t packetType = ACCEPTATION_PACKET;

std::vector<uint8_t> header = packet.pack(clientId, sequenceNumber, packetType);
// header will contain 13 bytes formatted according to the defined structure
```

### Packing a Body

```cpp
// Create the payload
std::vector<uint64_t> payload = {EVENT_PACKET, 0x01, 0x02};

// Serialize the body
std::vector<uint8_t> body = packet.pack(payload);
// The packet type (first element) determines the body format
```

### Unpacking a Packet

```cpp
// Receive data from the network
std::vector<uint8_t> receivedData = receiveFromNetwork();

// Unpack the packet
pm::PacketManager packet(0);
if (packet.unpack(receivedData)) {
    // Success - retrieve information
    uint8_t clientId = packet.getIdClient();
    uint8_t type = packet.getType();
    uint32_t seqNum = packet.getSequenceNumber();
    std::vector<uint64_t> payload = packet.getPayload();

    // Process according to type
    switch(type) {
        case CONNECTION_CLIENT_PACKET:
            handleConnection(payload);
            break;
        case EVENT_PACKET:
            handleEvent(payload);
            break;
        // ...
    }
} else {
    // Decoding error
    std::cerr << "Failed to unpack packet" << std::endl;
}
```

### Resetting a Packet

```cpp
// Reset all fields to their default values
packet.reset();
// Magic number: 0x93
// Client ID: 0
// Sequence number: 0
// Type: NO_OP_PACKET (0x00)
// Length: 0
// Payload: empty
```

## Dynamic Loading

### Via C API

```cpp
extern "C" {
    void *createPacketInstance(unsigned int id);
    int getType();
}
```

### Usage Example

```cpp
#include <dlfcn.h>

// Load the library
void* handle = dlopen("libPacket.so", RTLD_LAZY);
if (!handle) {
    std::cerr << "Cannot load library: " << dlerror() << std::endl;
    return;
}

// Retrieve the creation function
auto createPacket = (void* (*)(unsigned int))dlsym(handle, "createPacketInstance");
if (!createPacket) {
    std::cerr << "Cannot load symbol: " << dlerror() << std::endl;
    dlclose(handle);
    return;
}

// Create an instance
pm::IPacketManager* packet = static_cast<pm::IPacketManager*>(createPacket(1));

// Use the packet
// ...

// Clean up
delete packet;
dlclose(handle);
```

## Error Handling

### Data Validation

The system automatically validates:
- **Magic Number**: Must be 0x93
- **End Marker**: Must be `\r\n`
- **Packet size**: Must match the type
- **Packet type**: Must be recognized

### Return Codes

```cpp
bool unpack(std::vector<uint8_t> data);
// true  : Packet decoded successfully
// false : Decoding error (detailed logs on stderr)
```

### Error Messages

```
[SERVER] Error: unpack(): Empty packet data
[SERVER] Error: unpackHeader(): Invalid header size
[SERVER] Error: unpackHeader(): Invalid end of packet
[SERVER] Error: unpackBody(): Type mismatch
[SERVER] Error: unpackBody(): Unknown packet type received
[SERVER] Error: Payload have invalid length
```

## Packet Types Details

### CONNECTION_CLIENT_PACKET (0x01)

**Body size**: 11 bytes

**Format**:
```
+------+------+------+------+------+------+------+------+------+------+------+
| Type | Data | Data | Data | Data | Data | Data | Data | Data | \r   | \n   |
+------+------+------+------+------+------+------+------+------+------+------+
```

**Usage**: Connection request sent by the client to the server.

### ACCEPTATION_PACKET (0x02)

**Body size**: 4 bytes

**Format**:
```
+------+------+------+------+
| Type | Data | \r   | \n   |
+------+------+------+------+
```

**Usage**: Connection confirmation sent by the server to the client.

### DISCONNECTION_PACKET (0x03)

**Body size**: 4 bytes

**Format**:
```
+------+------+------+------+
| Type | Data | \r   | \n   |
+------+------+------+------+
```

**Usage**: Disconnection notification (client or server).

### EVENT_PACKET (0x04)

**Body size**: 5 bytes

**Format**:
```
+------+------+------+------+------+
| Type | Evt1 | Evt2 | \r   | \n   |
+------+------+------+------+------+
```

**Usage**: Game events (keys, actions, etc.).

## Performance and Optimization

### Reuse Pattern

```cpp
// Create once
pm::PacketManager packet(0);

// Reuse for multiple packets
for (auto& message : messages) {
    packet.reset();
    packet.setPayload(message);
    auto data = packet.pack(message);
    sendToNetwork(data);
}
```

### Memory Management

- `std::vector<uint8_t>` uses dynamic allocation
- RAII pattern ensures automatic resource cleanup
- No memory leaks if the interface is used correctly

## Tests

The library includes a comprehensive unit test suite covering:

- ✅ Sérialisation/Désérialisation (Big-Endian et Little-Endian)
- ✅ Pack/Unpack de tous les types de paquets
- ✅ Validation des données
- ✅ Gestion d'erreurs
- ✅ Round-trip tests
- ✅ Edge cases

```bash
./scripts/run_unit_tests.sh
```

## Exemple Complet

```cpp
#include "PacketManager.hpp"

void sendConnectionRequest() {
    // 1. Créer le packet manager
    pm::PacketManager packet(1);

    // 2. Préparer le payload de connexion
    std::vector<uint64_t> connectionData = {
        CONNECTION_CLIENT_PACKET,
        0x01, 0x02, 0x03, 0x04,
        0x05, 0x06, 0x07, 0x08
    };

    // 3. Créer le header
    std::vector<uint8_t> header = packet.pack(42, 1, CONNECTION_CLIENT_PACKET);

    // 4. Créer le body
    std::vector<uint8_t> body = packet.pack(connectionData);

    // 5. Combiner header + body
    std::vector<uint8_t> fullPacket;
    fullPacket.insert(fullPacket.end(), header.begin(), header.end());
    fullPacket.insert(fullPacket.end(), body.begin(), body.end());

    // 6. Envoyer sur le réseau
    sendToServer(fullPacket);
}

void receiveConnectionResponse(std::vector<uint8_t> data) {
    pm::PacketManager packet(0);

    // Décoder le header
    std::vector<uint8_t> header(data.begin(), data.begin() + HEADER_SIZE);
    if (!packet.unpack(header)) {
        std::cerr << "Invalid header" << std::endl;
        return;
    }

    // Vérifier le type
    if (packet.getType() == ACCEPTATION_PACKET) {
        // Décoder le body
        std::vector<uint8_t> body(data.begin() + HEADER_SIZE, data.end());
        if (packet.unpack(body)) {
            auto payload = packet.getPayload();
            std::cout << "Connected! Server assigned ID: "
                      << payload[0] << std::endl;
        }
    }
}
```

## Bonnes Pratiques

1. **Toujours valider** les données reçues du réseau
2. **Réutiliser** les instances de PacketManager avec `reset()`
3. **Vérifier les codes de retour** de `unpack()`
4. **Utiliser le bon endianness** selon la plateforme
5. **Logger les erreurs** pour faciliter le débogage
6. **Tester les edge cases** (paquets vides, données corrompues, etc.)

## Dépendances

- **ERROR** : Bibliothèque de gestion d'erreurs (pour `PacketError`)
- **Standard Library** : `<vector>`, `<memory>`, `<map>`, `<functional>`

## Compilation

```bash
# Via le script de compilation
./scripts/compile_project.sh

# Manuellement avec CMake
cd build
cmake ..
make Packet

# La bibliothèque sera générée dans build/libs/Packet/
# Et copiée automatiquement dans librairies/
```

## Conclusion

Le système de Packet Management fournit une solution robuste et flexible pour la communication réseau dans R-Type. L'utilisation d'une bibliothèque partagée permet une maintenance facile et une cohérence garantie entre le client et le serveur.