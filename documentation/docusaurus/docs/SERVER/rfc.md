---
sidebar_position: 2
---

# RFC Project explanation

Here in this page you will find the explanations of our rfc in our R-Type game.
You can also download it in pdf format [here](../../static/pdfs/rfc-r-type.pdf)



## Multiplayer Game Synchronization Protocol (PSJM)

**Status of This Memo**

   This document is not an Internet Standards Track specification; it is
   published for informational purposes.

   This document is a product of EPITECH Network Programming course.
   It represents information that the author believes is valuable to
   share with the community.

**Abstract**

   This document specifies the Multiplayer Game Synchronization Protocol
   (PSJM), a simple UDP-based protocol for real-time multiplayer games.
   It facilitates player connections/disconnections, position
   synchronization, and game state updates.

**Table of Contents**

   1. Introduction ................................................ 2
   2. Packet Format ............................................... 2
   3. Packet Types ................................................ 3
   4. Packet Details .............................................. 3
   5. Communication Example ....................................... 6
   5.1 Ready System Logic ......................................... 6
   6. Packet lost consideration ................................... 7
   7. Technical Considerations .................................... 8
   8. Map Format Protocol ......................................... 8
   9. References .................................................. 9
   10. Author's Address ........................................... 9



**1. Introduction**

   The Multiplayer Game Synchronization Protocol (PSJM) is a simple
   UDP-based protocol for real-time multiplayer games. It supports:

   - Player connections/disconnections
   - Position synchronization
   - Game state updates

**2. Packet Format**

   All packets have a fixed 6-byte header:

```    0                   1                   2
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | Magic | ID | Sequence |  Type  |    Length    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                    Payload                    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```
   Fields:
   - Magic Number
   - Client ID
   - Sequence Number
   - Length of Paylaod
   - Paylod

   Header:
   - Magic Number
   - Client ID
   - Sequence Number
   - Lenght

   Body:
   - Payload


**3. Packet Types**
Client and Server packet types (complete list used by the codebase):

```
   +--------+---------------------------+----------------------------------------+
   | Value  | Name                      | Description                            |
   +--------+---------------------------+----------------------------------------+
   | 0x00   | NO_OP_PACKET              | No operation / keep-alive              |
   | 0x01   | CONNECTION_CLIENT_PACKET  | Client connection request (name)       |
   | 0x02   | ACCEPTATION_PACKET        | Server acceptance / assign client ID   |
   | 0x03   | DISCONNECTION_PACKET      | Client disconnection                   |
   | 0x04   | EVENT_PACKET              | Client input/event                     |
   | 0x05   | GAME_STATE_PACKET         | Server game state update               |
   | 0x08   | END_GAME_PACKET           | Server notifies end of game / winner   |
   | 0x09   | CAN_START_PACKET          | Server tells clients they can start    |
   | 0x0A   | CLIENT_READY_PACKET       | Client signals ready state             |
   | 0x0B   | SPAWN_PLAYER_PACKET       | Server spawns a player/entity          |
   | 0x0C   | DEATH_PLAYER_PACKET       | Server notifies a player/entity death  |
   | 0x0D   | WHOAMI_PACKET             | Optional identification/resync packet  |
   | 0x0E   | SERVER_STATUS_PACKET      | Server sends lobby status information  |
   +--------+---------------------------+----------------------------------------+
```

**4. Packet Details**

4.1 Client Details

4.1.1 CONNECTION_CLIENT_PACKET (0x01) – Sent from client to server

   - Player name (UTF-8, max 8 chars + null terminator)
   - Fixed length: `LENGTH_CONNECTION_PACKET` (8 bytes)

4.1.2 DISCONNECTION_PACKET (0x03) – Client requests to disconnect

   - Player ID (1 byte)
   - Fixed length: `LENGTH_DISCONNECTION_PACKET` (1 byte)

4.1.3 EVENT_PACKET (0x04) – Client notifies input
   - Event type (1 byte, e.g., Up, Down, Left, Right, Space)
   - Additional event data (e.g., movement depth)
   - Fixed length: `LENGTH_EVENT_PACKET` (9 bytes)

4.1.4 CLIENT_READY_PACKET (0x0A) – Client signals it is ready
   - Used by client to indicate readiness prior to start


4.2 Server Details

4.2.1 ACCEPTATION_PACKET (0x02) – Sent from Server to Client (connection accept)

   - Player ID assigned by server (1 byte)
   - Fixed length: `LENGTH_ACCEPTATION_PACKET` (1 byte)

4.2.2 GAME_STATE_PACKET (0x05) – Server sends game state to clients

   - Contains state of entities (position, velocity, state...) serialized

4.2.3 END_GAME_PACKET (0x08) – Server notifies end of game and winner

   - Player ID who won (1 byte)
   - Fixed length: `LENGTH_END_GAME_PACKET` (1 byte)

4.2.4 CAN_START_PACKET (0x09) – Server tells clients the game can start

4.2.5 SPAWN_PLAYER_PACKET (0x0B) – Server spawns a player/entity

   - Payload includes entity data required for client to instantiate the entity

4.2.8 DEATH_PLAYER_PACKET (0x0C) – Server notifies a player/entity death

   - Payload describing the dead entity (identified, e.g., by ID)
   - Fixed length: `LENGTH_DEATH_PACKET` (8 bytes)

4.2.9 WHOAMI_PACKET (0x0D) – Optional identification / resynchronization packet

   - May be used to request/confirm identification or small resync actions
   - Fixed length: `LENGTH_WHOAMI_PACKET` (0 bytes)

4.2.10 SERVER_STATUS_PACKET (0x0E) – Server sends lobby status information

   - Connected clients count (8 bytes, uint64_t)
   - Ready clients count (8 bytes, uint64_t)
   - Client ID (8 bytes, uint64_t)
   - Client ready status (8 bytes, uint64_t, 0=not ready, 1=ready)
   - Fixed length: `LENGTH_SERVER_STATUS_PACKET` (32 bytes)
   - Sent periodically to keep clients updated on lobby state

4.2.11 NO_OP_PACKET (0x00) – No operation / keep-alive

   - Used when there is nothing to send; helps keep sequence numbers in sync

Notes:
- The canonical constant names and packet lengths are defined in `libs/Packet/IPacketManager.hpp`.
- The RFC tables above were aligned to match the names used in the codebase (both client and server). Where the original RFC used a different label (for example `CONNECTIONS`), the equivalent code name `ACCEPTATION_PACKET` is used here for clarity.


**5. Communication Example**

```
   +------------------+                      +------------------+
   |                  |                      |                  |
   |      Client      |                      |      Server      |
   |                  |                      |                  |
   +------------------+                      +------------------+
           |                                          |
           |             CONNECTION_CLIENT            |
           |----------------------------------------->|
           |                                          |
           |               ACCEPTATION                |
           |<-----------------------------------------|
           |                                          |
           |               CLIENT_READY               |
           |----------------------------------------->|
           |                                          |
           |               SERVER_STATUS              |
           |<-----------------------------------------|
           |                                          |
           |                CAN_START                 |
           |<-----------------------------------------|
           |                                          |
           |============= Game Loop Begins ===========|
           |                                          |
           |                  EVENT                   |
           |----------------------------------------->|
           |                                          |
           |               GAME_STATE                 |
           |<-----------------------------------------|
           |                                          |
           |                END_GAME                  |
           |<-----------------------------------------|
           |                                          |
           |              DISCONNECTION               |
           |----------------------------------------->|

```

**5.1 Ready System Logic**

After connection establishment, clients must signal readiness before the game begins. This ensures all players start simultaneously.

**Flow:**
1. Client connects and receives ACCEPTATION
2. Client loads necessary resources and displays ready interface
3. Client sends CLIENT_READY when player indicates readiness
4. Server tracks readiness status for each client
5. Server periodically sends SERVER_STATUS to update lobby information
6. When all connected clients are ready, server broadcasts CAN_START
7. Game loop begins with synchronized start

**Benefits:**
- Synchronized game starts across all clients
- Prevents clients from starting prematurely
- Provides lobby status updates

**6. Packet lost consideration**

6.1 Tracking :

    To avoid and track easier what was lost, each packet is numbered
    and assigned to a user so that the server can now when a package
    was lost.

6.2 Rollback :

    The client will have an interpalation logic, so that if needed he
    can predict and advance until the server (the absolute truth),
    sends a new packet or starts reponding again.

**7. Technical Considerations**

   - Encoding: UTF-8 text
   - Number format: Network order (big-endian)
   - Frequency: ?

**8. Map Format Protocol**

8.1. File Formatting

   - File type :
        File containing the map must be a .json

8.2. Map Format

   - Element type ?


8.3. Map Rendering

   - One elem = ?x? pixel square



**9. References**

   [RFC7322] Flanagan, H. and S. Ginoza, "RFC Style Guide", RFC 7322,
             DOI 10.17487/RFC7322, September 2014,
             [rfc](https://www.rfc-editor.org/info/rfc7322).

**10. Author's Address**

   Matisse Marsac
   EPITECH
   Email: matisse.marsac@epitech.eu

   Albane Merian
   EPITECH
   Email: albane.merian@epitech.eu

   Marin Lamy
   EPITECH
   Email: marin.lamy@epitech.eu

   Eliott Tesnier
   EPITECH
   Email: eliott.tesnier@epitech.eu

   Alban Roussee
   EPITECH
   Email: alban.roussee@epitech.eu
