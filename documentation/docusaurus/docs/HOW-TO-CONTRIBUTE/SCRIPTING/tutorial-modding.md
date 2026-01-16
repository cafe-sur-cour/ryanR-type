---
sidebar_position: 3
---

# Tutorial: Creating a Custom Enemy

In this tutorial, you will learn how to create a custom enemy behavior using Lua scripting. We will create a "Stalker" enemy that follows the vertical position of the nearest player and shoots at them.

## Step 1: The JSON Definition

First, you need to define the entity in a JSON file (e.g., `configs/entities/enemies/stalker.json`). This tells the engine which components the entity has, including the `ScriptingComponent` that points to our script.

```json
{
  "name": "stalker",
  "components": {
    "TransformComponent": {
      "scale": { "x": 3.0, "y": 3.0 }
    },
    "VelocityComponent": { "x": 0, "y": 0 },
    "SpeedComponent": { "speed": 150.0 },
    "HealthComponent": { "health": 50 },
    "HitboxRenderComponent": { "color": "red" },
    "ShootingStatsComponent": {
      "fireRate": 1.0,
      "shotCount": 1,
      "spreadAngle": 0
    },
    "ProjectilePrefabComponent": { "prefabName": "basic_shot" },
    "ScriptingComponent": {
      "scriptPath": "configs/scripts/stalker_behavior.lua"
    }
  }
}
```

## Step 2: The Lua Script

Create the file `configs/scripts/stalker_behavior.lua`. This is where we will write the logic.

### 2.1 Initialization

The `init` function is called when the enemy spawns. We can use it to set up initial state, though often it's empty if everything is set in JSON.

```lua
function init(entityId)
    print("Stalker spawned: " .. entityId)
end
```

### 2.2 The Update Loop

The `update` function is responsible for the AI. We want the enemy to:
1. Find the nearest player.
2. Move vertically to align with the player.
3. Aim and shoot at the player.

```lua
function update(entityId, deltaTime)
    -- Get my position
    local myX, myY = getEntityPosition(entityId)

    -- Find the target (player)
    local targetX, targetY = getNearestPlayerPosition(entityId)

    -- If no player is found (0,0), do nothing
    if targetX == 0 and targetY == 0 then
        return
    end

    -- 1. Movement Logic: Follow Y
    moveTowardsPlayer(entityId, myY, targetY)

    -- 2. Shooting Logic: Aim at player
    shootAtPlayer(entityId, myX, myY, targetX, targetY)
end

function moveTowardsPlayer(entityId, myY, targetY)
    local speed = getEntitySpeed(entityId)
    local diffY = targetY - myY

    -- Add a small "deadzone" to prevent jittering
    if math.abs(diffY) > 5.0 then
        -- Determine direction (1 for down, -1 for up)
        local dirY = diffY / math.abs(diffY)

        -- Create a movement intent (X=0, Y=direction * speed)
        -- Note: createMoveIntent takes velocity components
        createMoveIntent(entityId, 0, dirY * speed)
    end
end

function shootAtPlayer(entityId, myX, myY, targetX, targetY)
    -- Calculate angle to target
    local dx = targetX - myX
    local dy = targetY - myY

    -- arc tangent gives radians, convert to degrees
    local angleRad = math.atan(dy, dx)
    local angleDeg = angleRad * 180 / math.pi

    -- The engine handles fire rate cooldowns automatically
    createShootIntent(entityId, angleDeg)
end
```

### 2.3 Handling Death

When the enemy dies, we want it to spawn an explosion effect.

```lua
function death(entityId)
    local x, y = getEntityPosition(entityId)

    -- Offset the explosion to center it (optional, depends on sprite sizes)
    spawnEntity("explode", x, y)

    print("Stalker died!")
end
```

## Step 3: Testing

1. Add your new "stalker" enemy to a level file (e.g., `configs/map/level1.json`) in a wave.
2. Run the game server and client.
3. When the enemy spawns, it should track your movement and fire at you.
