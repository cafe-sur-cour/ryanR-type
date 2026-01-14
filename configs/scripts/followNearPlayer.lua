function init(entity)
end

function update(entity, dt)
    local speed = getEntitySpeed(entity)
    local px, py = getNearestPlayerPosition(entity)
    local ex, ey = getEntityPosition(entity)

    local dx = px - ex
    local dy = py - ey
    local dist = math.sqrt(dx * dx + dy * dy)

    if dist < 300 then
        local moveX = (dx / dist) * speed
        local moveY = (dy / dist) * speed
        createMoveIntent(entity, moveX, moveY)
    end
end

function death(entity)
end