function init(entity)
end

function update(entity, deltaTime)
    local ex, ey = getEntityPosition(entity)
    local px, py = getNearestPlayerPosition(entity)
    local speed = getEntitySpeed(entity)

    move(entity, speed, 0, py - ey)
    shoot(entity , px, py, ex, ey)
end

function move(entity, speed, dirX, dy)
    if speed > 0 then
        if math.abs(dy) > 1 then
            local dirY = dy / math.abs(dy)
            createMoveIntent(entity, 0, dirY * speed)
        end
    end
end

function shoot(entity, px, py, ex, ey)
    local dx = px - ex
    local dy = py - ey
    local angle_rad = math.atan(dy, dx)
    local angle_deg = angle_rad * 180 / math.pi
    createShootIntent(entity, angle_deg)
end

function death(entity)
    local px, py = getEntityPosition(entity)
    local entitySizeX, entitySizeY = getEntitySize(entity)
    local explosionSizeX, explosionSizeY = getEntitySize("explode")
    px = px + entitySizeX / 2 - 298 / 2
    py = py + entitySizeY / 2 - 217 / 2
    spawnEntity("explode", px, py)
end