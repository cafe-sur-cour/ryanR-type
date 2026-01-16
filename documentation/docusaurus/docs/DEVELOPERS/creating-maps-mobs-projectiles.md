---
sidebar_position: 2
---

# Creating Maps, Mobs and Projectiles via JSON Configs

This guide explains how to add/modify maps, enemies (mobs) and projectiles using JSON files in the `configs/` folder.

## Main Locations
- Entity prefabs (mobs, bosses, power-ups): `configs/entities/enemies/`, `configs/entities/boss/`, `configs/entities/powerUp/`
- Projectiles / projectile prefabs: often defined in `configs/entities/projectiles/` or included in the entity prefab via `ProjectilePrefabComponent`
- Scripts (behavior): `configs/scripts/` (Lua files)
- Maps / waves: `configs/map/` (e.g. `level1.json`)

## General Principles
- Entities are defined by JSON files that list components (HealthComponent, DamageComponent, ScriptingComponent, ShootingStatsComponent, ProjectilePrefabComponent, etc.).
- Dynamic behavior is implemented in Lua (in `configs/scripts/`). Common hooks are: `init`, `update`, `death`, `OnInteract`, `ActivateOrDeactivateForce`, `addForceLevel`.
- Maps reference prefabs by name in the `waves` (for enemies) and `powerUps` arrays.

## Steps to Add a New Enemy (Mob)
1. Create a JSON file in `configs/entities/enemies/` (e.g. `my_mob.json`).
2. Declare the necessary components:

```json
{
  "name": "my_mob",
  "components": {
    "HealthComponent": { "hp": 120 },
    "DamageComponent": { "damage": 20 },
    "ScoreComponent": { "score": 250 },
    "ScriptingComponent": { "scriptPath": "configs/scripts/my_mob.lua" },
    "ShootingStatsComponent": { "fireRate": 0.5, "shotCount": 1, "spread": 0 },
    "ProjectilePrefabComponent": { "prefab": "basic_bullet" }
  }
}
```

3. (Optional) Add the behavioral script `configs/scripts/my_mob.lua` and expose the `init`/`update`/`death` functions as needed. The engine calls these functions via the `ScriptingComponent`.

4. Add an entry in the map to spawn the enemy:

```json
{
  "waves": [
    {
      "spawn_time": 8.0,
      "prefab": "my_mob",
      "position": { "x": 900, "y": 200 }
    }
  ]
}
```

## Steps to Add a Projectile
1. Create a projectile prefab (if your project uses a dedicated folder): `configs/entities/projectiles/basic_bullet.json`.
2. Minimal projectile example:

```json
{
  "name": "basic_bullet",
  "components": {
    "DamageComponent": { "damage": 10 },
    "ProjectileComponent": { "speed": 700, "direction": { "x": -1, "y": 0 } },
    "ColliderComponent": { "radius": 6 }
  }
}
```

3. Reference this prefab from an enemy via `ProjectilePrefabComponent` or dynamically create the projectile in the Lua script with the `createShootIntent` / `createMoveIntent` API.

## Best Practices
- Name your prefabs clearly (`enemy_snake_head`, `boss_blaster`, `powerup_force_level`).
- Separate data (JSON) from logic (Lua). JSONs describe static components, scripts handle temporal behaviors and phase transitions.
- Test incrementally: add the entity to a debug map (early spawn) to verify collisions / shots / animations.
- Respect existing constants and hooks: see `common/constants.hpp` for expected scripting function names (e.g. `ONINTERACT_FUNCTION`, `INIT_FUNCTION`).

## Useful Tools
- Visualize a level by opening `configs/map/level1.json` and searching for `powerUps` / `waves` for concrete examples.
- Consult existing scripts (`configs/scripts/force.lua`, `boss1.lua`, `flyingForce.lua`) for reusable patterns.

## Quick Troubleshooting
- If an entity doesn't spawn: check the `prefab` name in the map, validate the JSON (format) and look at server logs (`[SERVER]` messages).
- If a Lua script doesn't execute: verify the `scriptPath` in the `ScriptingComponent` and that the script implements expected hooks.

## Final Notes
- This guide covers the basic flow. For advanced behaviors (composite bosses, segments, attached forces), study existing scripts in `configs/scripts/` which show complete examples of scripting API and `ResourceManager` interaction.

Reference files: `configs/entities/enemies/*`, `configs/entities/boss/*`, `configs/entities/powerUp/*`, `configs/entities/projectiles/*`, `configs/scripts/*`, `configs/map/*`.

