---
title: Map Parser
---

## Overview

The MapParser is responsible for parsing map configuration files and instantiating game entities based on the map layout. It handles background entities, tile-based grids, and enemy waves.

## Location
`common/Parser/MapParser/`

## Key Concepts

### Main Class: MapParser

**Constructor:**
```cpp
MapParser(std::shared_ptr<EntityPrefabManager> prefabManager,
          std::shared_ptr<ecs::Registry> registry)
```

**Important Methods:**
- `void parseMapFromFile(const std::string& filePath)`: Parses a JSON map file and creates entities
- `void parseMap(const nlohmann::json& mapJson)`: Parses map data from a JSON object

## Map File Format

Map files are JSON documents that can contain the following sections:

### Background Configuration
```json
{
  "background": "background_prefab_name",
  "scrollSpeed": 2.5
}
```

- `background`: Name of the prefab to use for the background entity
- `scrollSpeed`: Speed at which the background scrolls (affects velocity component)

### Legend and Map Grid
```json
{
  "legend": {
    "W": "wall_prefab",
    ".": "empty",
    "P": "player_prefab"
  },
  "map": [
    "W W W W W",
    "W . . . W",
    "W . P . W",
    "W . . . W",
    "W W W W W"
  ]
}
```

- `legend`: Maps single characters to prefab names
- `map`: Array of strings representing the grid layout
- Each character in the map corresponds to a prefab from the legend
- `"empty"` is a special prefab name that creates no entity

### Enemy Waves
```json
{
  "waves": [
    {
      "spawnLength": 100,
      "posX": 50,
      "enemies": [
        {
          "type": "enemy1_prefab",
          "count": 3
        },
        {
          "type": "enemy2_prefab",
          "count": 2
        }
      ]
    }
  ]
}
```

- `waves`: Array of wave configurations
- `spawnLength`: Width of the spawn area in tiles
- `posX`: X position offset for the wave
- `enemies`: Array of enemy groups to spawn
- `type`: Prefab name for the enemy
- `count`: Number of enemies to spawn of this type

## Entity Creation Process

### Background Entity
- Created at position (0, 0)
- If the prefab has a VelocityComponent, sets its velocity based on scrollSpeed
- Velocity calculation: `(-scrollSpeed * -1.0f, 0.0f)` (scrolls left)

### Grid Entities
- Position calculated as: `(col * tileWidth, row * tileHeight)`
- Only created if the prefab exists in EntityPrefabManager
- Skips entities marked as "empty"

### Wave Entities
- Random Y position between 0 and MAX_HEIGHT (1080.0f)
- X position calculated with random offset within spawnLength
- Formula: `posX * tileWidth + random(0, 1) * spawnLength * tileWidth`

## Error Handling

The MapParser includes comprehensive error handling:

- **File not found**: Throws `ParserError::FILE_NOT_FOUND`
- **Invalid JSON**: Throws `ParserError::INVALID_FORMAT`
- **Missing prefabs**: Logs warnings and skips entity creation
- **Invalid data types**: Logs errors and skips problematic sections
- **Malformed waves**: Logs warnings and skips invalid waves

## Usage Example

```cpp
// Setup
auto registry = std::make_shared<ecs::Registry>();
auto prefabManager = std::make_shared<EntityPrefabManager>();
auto mapParser = std::make_shared<MapParser>(prefabManager, registry);

// Parse a map file
try {
    mapParser->parseMapFromFile("maps/level1.json");
} catch (const err::ParserError& e) {
    std::cerr << "Failed to parse map: " << e.what() << std::endl;
}
```
