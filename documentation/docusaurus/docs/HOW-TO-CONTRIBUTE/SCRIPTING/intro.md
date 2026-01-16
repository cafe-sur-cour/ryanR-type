---
sidebar_position: 1
---

# Introduction to Scripting

The R-Type engine uses **Lua** to script the behavior of game entities, such as enemies, bosses, projectiles, and power-ups. This allows for dynamic and customizable game logic without modifying the core C++ engine.

## How it Works

Each entity in the game can have a `ScriptingComponent`, which points to a Lua script file. The engine loads this script and executes specific functions (hooks) at different stages of the entity's lifecycle.

The mapping is defined in the entity's JSON configuration file. For example:

```json
{
  "components": {
    "ScriptingComponent": {
      "scriptPath": "configs/scripts/my_enemy.lua"
    }
    // ... other components
  }
}
```

## Lifecycle Hooks

The engine looks for the following global functions in your Lua script:

### `init(entityId)`
Called once when the entity is spawned. Use this to initialize state, set custom properties, or log debug information.

### `update(entityId, deltaTime)`
Called every frame (or tick). This is where the main logic lives: movement, checking for players, deciding to shoot, etc.
- `entityId`: The unique ID of the entity running the script.
- `deltaTime`: The time elapsed since the last frame (in seconds).

### `death(entityId)`
Called when the entity is destroyed (e.g., health reaches zero). Use this to spawn death effects, split into smaller enemies, or increment score.

### `OnInteract(entityId)`
Called when an interaction occurs (specific to interactable entities).

### `ActivateOrDeactivateForce(entityId, active)`
Used for Force entities to toggle their state.

### `addForceLevel(entityId)`
Used to upgrade a Force entity.

## The Scripting API

The engine exposes a rich set of C++ functions to Lua, allowing you to control entities, spawn new ones, query game state, and more. See the [API Reference](./api-reference.md) for a complete list of available functions.

In addition to the game-specific API, you have access to the standard Lua **base**, **math**, and **table** libraries.
