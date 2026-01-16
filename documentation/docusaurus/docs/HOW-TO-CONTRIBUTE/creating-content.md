---
sidebar_position: 4
---

# Creating Maps, Mobs and Projectiles

This guide explains how to add and modify game content (maps, enemies, projectiles) using the JSON configuration system.

## Overview

The game content is data-driven, defined in JSON files located in the `configs/` directory:

- **Entity Prefabs**: `configs/entities/` (enemies, bosses, power-ups, projectiles)
- **Maps / Levels**: `configs/map/`
- **Scripts**: `configs/scripts/` (See [Scripting & Configuration](./SCRIPTING/intro.md))

## Adding a New Enemy (Mob)

To create a new enemy, you define its components in a JSON file.

1.  **Create the File**: `configs/entities/enemies/my_new_mob.json`
2.  **Define Components**:

```json
{
  "name": "my_new_mob",
  "components": {
    "TransformComponent": { "scale": { "x": 2, "y": 2 } },
    "HealthComponent": { "health": 50 },
    "DamageComponent": { "damage": 10 },
    "ScoreComponent": { "score": 100 },
    "HitboxRenderComponent": { "color": "green" }, 
    "ScriptingComponent": { "scriptPath": "configs/scripts/my_mob_behavior.lua" },
    "ShootingStatsComponent": { 
        "fireRate": 1.5, 
        "shotCount": 1, 
        "spreadAngle": 0 
    },
    "ProjectilePrefabComponent": { "prefabName": "basic_enemy_shot" }
  }
}
```

3.  **Spawn in Map**: Add it to a wave in a level file (e.g., `configs/map/level1.json`).

```json
{
  "waves": [
    {
      "spawn_time": 5.0,
      "prefab": "my_new_mob",
      "position": { "x": 1800, "y": 500 }
    }
  ]
}
```

## Creating Projectiles

Projectiles are also entities. Defines them in `configs/entities/projectiles/`.

```json
{
  "name": "basic_enemy_shot",
  "components": {
    "TransformComponent": { "scale": { "x": 1, "y": 1 } },
    "VelocityComponent": { "x": -500, "y": 0 },
    "DamageComponent": { "damage": 10 },
    "ColliderComponent": { "radius": 5 },
    "HitboxRenderComponent": { "color": "yellow" },
    "LifetimeComponent": { "lifetime": 3.0 }
  }
}
```

## Map Composition

Maps are defined in `configs/map/`. A map contains:

-   **Background**: Scrolling background settings.
-   **Music**: Background track.
-   **Waves**: List of enemies to spawn at specific times.
-   **PowerUps**: List of powerups to spawn.

Example `level1.json`:

```json
{
  "mapLength": 120,
  "background": {
    "texturePath": "assets/sprites/background.png",
    "scrollSpeed": 100
  },
  "music": {
    "musicFile": "assets/musics/level1.ogg",
    "volume": 50
  },
  "waves": [
    ...
  ]
}
```
