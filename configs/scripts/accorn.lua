local accorn =
{
    hasSpawned = false
}

function init(entity)
end

function update(entity, deltaTime)
    local px, py = getNearestPlayerPosition(entity)
    local ex, ey = getEntityPosition(entity)
    local speed = getEntitySpeed(entity)

    local horizontalDistance = math.abs(px - ex)
    local isPlayerBelow = py > ey
    local isPlayerAligned = horizontalDistance < 100

    if isPlayerBelow and isPlayerAligned and accorn.hasSpawned == false then
        spawnEntity("helice", ex, ey)
        setAnimationState(entity, "base")
        accorn.hasSpawned = true
    end

    if accorn.hasSpawned then
        createMoveIntent(entity, 0, speed * 7)
    else
        createMoveIntent(entity, -1 * speed, 0)
    end
end

function death(entity)
end