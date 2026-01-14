function init(entity)
    _G["time_" .. entity] = 0
    local states = {"orange", "yellow", "orangeDark", "pink"}
    local randomIndex = math.random(1, #states)
    setAnimationState(entity, states[randomIndex])
end

function update(entity, deltaTime)
    _G["time_" .. entity] = (_G["time_" .. entity] or 0) + deltaTime
    local t = _G["time_" .. entity]
    local frequency = 2.0
    local amplitude = 50.0
    local oscillation = math.sin(t * frequency) * amplitude

    local speed = getEntitySpeed(entity)
    local dirX = -1.0
    local dirY = oscillation
    local length = math.sqrt(dirX * dirX + dirY * dirY)
    if length > 0 then
        dirX = dirX * speed
        dirY = (dirY / length) * speed
    end
    createMoveIntent(entity, dirX, dirY)
end

function death(entity)
    local px, py = getEntityPosition(entity)
    local entitySizeX, entitySizeY = getEntitySize(entity)
    local explosionSizeX, explosionSizeY = getEntitySize("explode")
    px = px + entitySizeX / 2 - 298 / 2
    py = py + entitySizeY / 2 - 217 / 2
    spawnEntity("explode", px, py)
end