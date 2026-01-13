Based on my analysis of the codebase and test structure, here's a breakdown of the parts that are not covered by unit tests:

Completely Untested Areas

## 1. Client Code (client directory)

All client-side logic is untested, including:

 - Core game loop (Core.cpp)
 - Network handling (ClientNetwork.cpp, ClientReceivedPacket.cpp, ClientSentPacket.cpp)
 - Settings management (SettingsManager.cpp, SettingsConfig.cpp)
 - Death animation system (DeathAnimationSpawner.cpp)
 - All client components, systems, UI, input handling, interpolation, and game state management
 - Main entry point (main.cpp)

## 2. Server Core Logic (server directory)

 - Main server implementation (Server.cpp)
 - Lobby management (Lobby.cpp)
 - Library loading (ServerLibsLoading.cpp)
 - Packet handling (ServerReceivePacket.cpp, ServerSentPacket.cpp)
 - ECS conversions (ECSConversions.cpp)
 - Delta tracking system (deltaTracker/)
 - Game state management (gsm/)
 - Resource initialization (initResourcesManager/)

## 3. Common Libraries (common directory)

 - Collision Rules (CollisionRules/) - No tests
 - Dynamic Library Loading (DLLoader/) - No tests
 - Game Rules (GameRules.cpp) - No tests
 - Input Mapping (InputMapping/) - No tests
 - Interfaces (interfaces/) - No tests
 - Resource Management (resourceManager/) - No tests
 - Signal System (Signal/) - No tests
 - Utilities (utils/) - No tests
 - Game State Management (gsm/) - No tests
 - Many Systems - Only a subset of systems are tested:
 - Untested: input/, interactions/, map/, scripting/, shooting/, systemManager/

## 4. Multimedia Library (Multimedia)

 - Asset management, SFML integration, event handling - completely untested

## 5. Network Library (Network)

Core networking abstractions, ASIO implementation, platform-specific code - completely untested

## 6. Main Executables

r-type_client and r-type_server executables - integration tested only, no unit tests


**Partially Tested Areas**

 - ECS Components: Most components have tests, but some may be missing
 - Packet System: Basic packet tests exist, but likely incomplete coverage
 - Parser: JSON and map parsing tested, but may not cover all edge cases
 - The codebase has good test coverage for core ECS functionality, basic types, and some server configuration, but the majority of application logic (client, server main loops, networking, multimedia, and many systems) remains untested. This represents a significant gap in test coverage that could impact code reliability and refactoring confidence.
