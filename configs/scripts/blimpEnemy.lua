function init(entity)
    _G["time_" .. entity] = 0
end

function update(entity, deltaTime)
    _G["time_" .. entity] = (_G["time_" .. entity] or 0) + deltaTime
    local t = _G["time_" .. entity]
    local frequency = 2.0
    local amplitude = 2.0
    local vertical = math.sin(t * frequency) * amplitude

    local speed = getEntitySpeed(entity)
    local dirX = 1.0
    local dirY = vertical
    local length = math.sqrt(dirX * dirX + dirY * dirY)
    if length > 0 then
        dirX = (dirX / length) * speed
        dirY = (dirY / length) * speed
    end
    createMoveIntent(entity, dirX, dirY)

    local px, py = getNearestPlayerPosition(entity)
    local ex, ey = getEntityPosition(entity)
    local dx = px - ex
    local dy = py - ey
    local angle = math.atan(dy, dx) * 180 / math.pi
    if createShootIntent(entity, angle) == true then
        setAnimationState(entity, "shooting")
    end
end

function death(entity)
    _G["time_" .. entity] = nil
    local px, py = getEntityPosition(entity)
    local entitySizeX, entitySizeY = getEntitySize(entity)
    local explosionSizeX, explosionSizeY = getEntitySize("explode")
    px = px + entitySizeX / 2 - 298 / 2
    py = py + entitySizeY / 2 - 217 / 2
    spawnEntity("explode", px, py)
end
