---
sidebar_position: 2
---

# API Reference

The following global functions are available in all Lua scripts.

## Movement & Transfrom

### `createMoveIntent(entityId, x, y)`
Creates an intent to move the entity in a specific direction.
- `entityId` (int): The entity to move.
- `x` (float): X direction component (-1 to 1).
- `y` (float): Y direction component (-1 to 1).

### `setPosition(entityId, x, y)`
Instantly teleports the entity to the specified coordinates.
- `entityId` (int): The entity ID.
- `x` (float): New X coordinate.
- `y` (float): New Y coordinate.

### `getEntityPosition(entityId)`
Returns the current position of an entity.
- `entityId` (int): The entity ID.
- **Returns**: `x` (float), `y` (float). Returns `0,0` if entity not found.

### `getEntityVelocity(entityId)`
Returns the current velocity of an entity.
- `entityId` (int): The entity ID.
- **Returns**: `x` (float), `y` (float).

### `setEntityRotation(entityId, rotation)`
Sets the rotation of the entity.
- `entityId` (int): The entity ID.
- `rotation` (float): Rotation in degrees.

### `getEntitySpeed(entityId)`
Gets the movement speed of the entity.
- `entityId` (int): The entity ID.
- **Returns**: `speed` (float).

### `setEntitySpeed(entityId, speed)`
Sets the movement speed of the entity.
- `entityId` (int): The entity ID.
- `speed` (float): New speed value.

### `getNearestPlayerPosition(entityId)`
Finds the position of the nearest player to the given entity.
- `entityId` (int): The entity performing the check.
- **Returns**: `x` (float), `y` (float). Returns `0,0` if no player found.

### `getNearestEnemyPosition(entityId)`
Finds the position of the nearest enemy (Mob) to the given entity.
- `entityId` (int): The entity performing the check.
- **Returns**: `x` (float), `y` (float).

### `getGameZonePosition()`
Gets the top-left position of the active game zone / camera.
- **Returns**: `x` (float), `y` (float).

### `getGameZoneSize()`
Gets the width and height of the game zone.
- **Returns**: `width` (float), `height` (float).

### `getGameZoneVelocity()`
Gets the current scrolling velocity of the game zone.
- **Returns**: `x` (float), `y` (float).

### `setGameZoneVelocity(x, y)`
Sets the scrolling velocity of the game zone.
- `x` (float): X velocity.
- `y` (float): Y velocity.

## Combat & Stats

### `createShootIntent(entityId, angle)`
Request the entity to shoot a projectile.
- `entityId` (int): The shooting entity.
- `angle` (float): The angle to shoot at (in degrees).
- **Returns**: `bool` (true if intent created, false if cooldown active).

### `setProjectilePrefab(entityId, prefabName)`
Changes the type of projectile the entity shoots.
- `entityId` (int): The entity ID.
- `prefabName` (string): The name of the projectile prefab (defined in JSON).

### `createDeathIntent(entityId, sourceId)`
Forces an entity to die.
- `entityId` (int): The entity to kill.
- `sourceId` (int): The entity responsible for the death (usually the caller, or 0).

### `setInvulnerable(entityId, isInvulnerable)`
Sets the invulnerability status of an entity.
- `entityId` (int): The entity ID.
- `isInvulnerable` (bool): `true` to make invulnerable, `false` otherwise.

### `getFireRate(entityId)`
Gets the fire rate (shots per second) of the entity.
- `entityId` (int): The entity ID.
- **Returns**: `rate` (float).

### `setFireRate(entityId, fireRate)`
Sets the fire rate of the entity.
- `entityId` (int): The entity ID.
- `fireRate` (float): Shots per second.

### `reverseShootOrientation(entityId)`
Flips the shooting pattern orientation by 180 degrees.
- `entityId` (int): The entity ID.

### `getMaxCharge(entityId)`
Gets the maximum charge level for charged shots.
- `entityId` (int): The entity ID.
- **Returns**: `maxCharge` (float).

### `setMaxCharge(entityId, maxCharge)`
Sets the maximum charge level.
- `entityId` (int): The entity ID.
- `maxCharge` (float): New maximum charge.

### `getChargeReloadTime(entityId)`
Gets the time required to reload a charged shot.
- `entityId` (int): The entity ID.
- **Returns**: `time` (float).

### `setChargeReloadTime(entityId, reloadTime)`
Sets the charged shot reload time.
- `entityId` (int): The entity ID.
- `reloadTime` (float): Time in seconds.

## Entity Management

### `spawnEntity(prefabName, x, y)`
Spawns a new entity from a prefab.
- `prefabName` (string): The name of the prefab to spawn.
- `x` (float): Spawn X coordinate.
- `y` (float): Spawn Y coordinate.
- **Returns**: `entityId` (int), or 0 on failure.

### `isEntityAlive(entityId)`
Checks if an entity is still active in the world.
- `entityId` (int): The entity ID.
- **Returns**: `bool`.

### `isEntityPlayer(entityId)`
Checks if the entity is a player.
- `entityId` (int): The entity ID.
- **Returns**: `bool`.

### `getEntityId(entityId)`
Returns the numeric ID of an entity. (Mostly identity function for Lua).
- `entityId` (int): The entity ID.
- **Returns**: `size_t`.

### `getEntitySize(entityId)`
Gets the dimensions of the entity's collider/sprite (scaled).
- `entityId` (int): The entity ID.
- **Returns**: `width` (float), `height` (float).

### `setAnimationState(entityId, newState)`
Changes the current animation state of the entity.
- `entityId` (int): The entity ID.
- `newState` (string): The name of the animation state (must match `states` key in component config).

## Hierarchy & Parts

### `addPartId(entityId, partId)`
Adds a child part to an entity.
- `entityId` (int): The parent entity.
- `partId` (int): The child entity to add.

### `removePartId(entityId, partId)`
Removes a child part from an entity.
- `entityId` (int): The parent entity.
- `partId` (int): The child entity to remove.

### `getEntityParts(entityId)`
Gets a list of IDs for all parts attached to the entity.
- `entityId` (int): The parent entity.
- **Returns**: `table` (list of integers).

### `setParentId(entityId, parentId)`
Sets the parent of an entity.
- `entityId` (int): The child entity.
- `parentId` (int): The new parent entity.

### `getParentId(entityId)`
Gets the parent ID of the entity.
- `entityId` (int): The child entity.
- **Returns**: `id` (int).

### `getOwner(entityId)`
Gets the owner ID of the entity (e.g., who fired the projectile).
- `entityId` (int): The entity ID.
- **Returns**: `id` (int).

## Force System

### `addForceLevel(entityId)`
Triggers the level-up logic for a Force entity attached to the given entity.
- `entityId` (int): The entity holding the force (usually a player).

### `countForcesByType(playerEntityId, forceType)`
Counts how many force modules of a specific type are attached to the player.
- `playerEntityId` (int): The player's entity ID.
- `forceType` (string): The type tag of the force.
- **Returns**: `count` (int).

### `getForcePositionByType(entityId, playerEntityId, forceType)`
Gets the index/position of a specific force entity among forces of the same type.
- `entityId` (int): The force entity ID.
- `playerEntityId` (int): The player entity ID.
- `forceType` (string): The force type.
- **Returns**: `index` (int).

## Utility

### `print(msg)`
Prints a message to the engine's standard output (console).
- `msg` (string): The message to print.
