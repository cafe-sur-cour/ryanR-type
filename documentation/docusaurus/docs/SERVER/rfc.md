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
   synchronization, and game state updates. The protocol has been extended
   to support user authentication, leaderboards, user profiles, level
   progression, and compressed game state transmission.

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
   - User authentication (login/register)
   - Leaderboard system
   - User profiles and statistics
   - Level progression
   - Compressed data transmission

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
   +--------+---------------------------+-------------------------------------------+
   | Value  | Name                      | Description                               |
   +--------+---------------------------+-------------------------------------------+
   | 0x00   | NO_OP_PACKET              | No operation / keep-alive                 |
   | 0x01   | CONNECTION_CLIENT_PACKET  | Client connection request (name)          |
   | 0x02   | ACCEPTATION_PACKET        | Server acceptance / assign client ID      |
   | 0x03   | DISCONNECTION_PACKET      | Client disconnection                      |
   | 0x04   | EVENT_PACKET              | Client input/event                        |
   | 0x05   | END_GAME_PACKET           | Server notifies end of game / winner      |
   | 0x06   | CAN_START_PACKET          | Server tells clients they can start       |
   | 0x07   | CLIENT_READY_PACKET       | Client signals ready state                |
   | 0x08   | SPAWN_PLAYER_PACKET       | Server spawns a player/entity             |
   | 0x09   | DEATH_PLAYER_PACKET       | Server notifies a player/entity death     |
   | 0x0A   | WHOAMI_PACKET             | Optional identification/resync packet     |
   | 0x0B   | SERVER_STATUS_PACKET      | Server sends lobby status information     |
   | 0X0C   | REQUEST_LOBBY_PACKET      | Client send request to create lobby       |
   | 0x0D   | SEND_LOBBY_CODE_PACKET    | Server sends the code to whom requested   |
   | 0X0E   | CONNECT_TO_LOBBY          | Client connect to an existing lobby       |
   | 0x0F   | LOBBY_MASTER_REQUEST_START| Client that created lobby starts the game |
   | 0x10   | LOBBY_CONNECT_VALUE       | Return sucess or failure of connection    |
   | 0x11   | LEVEL_COMPLETE_PACKET     | Server notifies level completion          |
   | 0x12   | NEXT_LEVEL_PACKET         | Server notifies next level transition     |
   | 0x13   | REGISTER_PACKET           | Client registration request               |
   | 0x14   | CONNECT_USER_PACKET       | Server confirms user authentication       |
   | 0x15   | LOGIN_PACKET              | Client login request                      |
   | 0x16   | GAME_STATE_BATCH_PACKET   | Server batched game state update          |
   | 0x17   | GAME_STATE_BATCH_COMPRESSED| Compressed batched game state             |
   | 0x18   | GAME_STATE_COMPRESSED     | Compressed game state update              |
   | 0x19   | REQUEST_LEADERBOARD       | Client requests leaderboard data          |
   | 0x1A   | LEADERBOARD_PACKET        | Server sends leaderboard information      |
   | 0x1B   | REGISTER_FAIL_PACKET      | Server notifies registration failure      |
   | 0x1C   | REQUEST_PROFILE           | Client requests user profile data         |
   | 0x1D   | PROFILE_PACKET            | Server sends user profile information     |
   | 0x1E   | GAME_RULES_PACKET         | Server sends current game rules to client |
   | 0x1F   | REQUEST_GAME_RULES_UPDATE | Client requests an update for game rules  |
   +--------+---------------------------+-------------------------------------------+
```

**4. Packet Details**

**4.1 Client Details**

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

4.1.4 CLIENT_READY_PACKET (0x08) – Client signals it is ready

   - Used by client to indicate readiness prior to start

4.1.5 REQUEST_LOBBY_PACKET (0x0D) - Client request a game code

   - Payload empty
   - Can be sent once

4.1.6 CONNECT_TO_LOBBY (0x0F) - Client send a request to connect to a lobby

   - Payload contains the lobby code
   - Fixed length `LENGTH_LOBBY_CODE_PACKET` (8 bytes)

4.1.7 LOBBY_MASTER_REQUEST_START (0x10) - Client that created the lobby can start the game

   - Payload contains the lobby code
   - Fixed length `LENGTH_LOBBY_CODE_PACKET` (8 bytes)

4.1.8 REGISTER_PACKET (0x14) – Client registration request

   - Username and password for account creation
   - Fixed length: `LENGTH_REGISTER_PACKET` (16 bytes)

4.1.9 LOGIN_PACKET (0x16) – Client login request

   - Username and password for authentication
   - Fixed length: `LENGTH_LOGIN_PACKET` (16 bytes)

4.1.10 REQUEST_LEADERBOARD (0x1A) – Client requests leaderboard data

   - Empty payload
   - Fixed length: `LENGTH_REQUEST_LEADERBOARD` (0 bytes)

4.1.11 REQUEST_PROFILE (0x1D) – Client requests user profile data

   - Empty payload
   - Fixed length: `LENGTH_REQUEST_PROFILE` (0 bytes)

4.1.12 REQUEST_GAME_RULES_UPDATE (0x20) – Client requests an update for game rules

   - Empty payload
   - Fixed length: `LENGTH_REQUEST_GAME_RULES_UPDATE` (0 bytes)

**4.2 Server Details**

4.2.1 ACCEPTATION_PACKET (0x02) – Sent from Server to Client (connection accept)

   - Player ID assigned by server (1 byte)
   - Fixed length: `LENGTH_ACCEPTATION_PACKET` (1 byte)

4.2.2 END_GAME_PACKET (0x06) – Server notifies end of game and winner

   - Player ID who won (1 byte)
   - Fixed length: `LENGTH_END_GAME_PACKET` (1 byte)

4.2.3 CAN_START_PACKET (0x07) – Server tells clients the game can start

4.2.4 SPAWN_PLAYER_PACKET (0x08) – Server spawns a player/entity

   - Payload includes entity data required for client to instantiate the entity

4.2.5 DEATH_PLAYER_PACKET (0x0A) – Server notifies a player/entity death

   - Payload describing the dead entity (identified, e.g., by ID)
   - Fixed length: `LENGTH_DEATH_PACKET` (8 bytes)

4.2.6 WHOAMI_PACKET (0x0B) – Optional identification / resynchronization packet

   - May be used to request/confirm identification or small resync actions
   - Fixed length: `LENGTH_WHOAMI_PACKET` (0 bytes)

4.2.7 SERVER_STATUS_PACKET (0x0C) – Server sends lobby status information

   - Connected clients count (8 bytes, uint64_t)
   - Ready clients count (8 bytes, uint64_t)
   - Client ID (8 bytes, uint64_t)
   - Client ready status (8 bytes, uint64_t, 0=not ready, 1=ready)
   - Fixed length: `LENGTH_SERVER_STATUS_PACKET` (32 bytes)
   - Sent periodically to keep clients updated on lobby state

4.2.8 SEND_LOBBY_CODE_PACKET (0x0E) Server sends the lobby code to the 'master' of the game

   - Payload contains the lobby code
   - Fixed length `LENGTH_LOBBY_CODE_PACKET` (8 bytes)

4.2.9 LOBBY_CONNECT_VALUE (0x11) Server says to the client if the connection to the lobby was succesfull or not

   - Payload contains char, t or f
   - Fixed length `LENGTH_LOBBY_CONNECT_VALLUE` (1 bytes)

4.2.10 LEVEL_COMPLETE_PACKET (0x12) – Server notifies level completion

   - Indicates that the current level has been completed
   - Fixed length: `LENGTH_LEVEL_COMPLETE_PACKET` (0 bytes)

4.2.11 NEXT_LEVEL_PACKET (0x13) – Server notifies next level transition

   - Indicates transition to the next level
   - Fixed length: `LENGTH_NEXT_LEVEL_PACKET` (0 bytes)

4.2.12 REGISTER_FAIL_PACKET (0x1C) – Server notifies registration failure

   - Contains error message for failed registration
   - Fixed length: `LENGTH_REGISTER_FAIL_PACKET` (variable)

4.2.13 LEADERBOARD_PACKET (0x1B) – Server sends leaderboard information

   - Contains player rankings and scores
   - Fixed length: `LENGTH_LEADERBOARD_PACKET` (variable)

4.2.14 PROFILE_PACKET (0x1E) – Server sends user profile information

   - Contains user statistics (wins, high score, games played)
   - Fixed length: `LENGTH_PROFILE_PACKET` (variable)

4.2.15 GAME_RULES_PACKET (0x1F) – Server sends current game rules to clients

   - Payload contains serialized game rules (e.g., difficulty multipliers, spawn toggles)
   - Fixed length: `LENGTH_GAME_RULES_PACKET` (variable)

Notes:
- The canonical constant names and packet lengths are defined in `common/interfaces/IPacketManager.hpp`.
- The RFC tables above were aligned to match the names used in the codebase (both client and server). Where the original RFC used a different label (for example `CONNECTIONS`), the equivalent code name `ACCEPTATION_PACKET` is used here for clarity.
- Additional packets have been added for user authentication, leaderboards, profiles, level progression, and compressed data transmission.


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
   - Compression: LZ4 compression for game state packets to reduce bandwidth usage


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
