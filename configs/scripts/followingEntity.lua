function init(entity)
end

function update(entity)
    local speed = getEntitySpeed(entity)
    local px, py = getNearestPlayerPosition(entity)
    local ex, ey = getEntityPosition(entity)

    local dx = px - ex
    local dy = py - ey
    local dist = math.sqrt(dx * dx + dy * dy)

    local moveX = (dx / dist) * speed
    local moveY = (dy / dist) * speed
    createMoveIntent(entity, moveX, moveY)
end
