isequipped = false
playerSizeX = 0
playerSizeY = 0
entitySizeX = 0
entitySizeY = 0

function init(entity)
end

function getForcePositionInList(entity, playerEntity)
    return getForcePositionByType(entity, playerEntity, "flyingForce")
end

function update(entity, deltaTime)
    if getParentId(entity) == 0 then
        return
    end
    local px, py = getEntityPosition(getParentId(entity))
    local forcePosition = getForcePositionInList(entity, getParentId(entity))
    local targetX = px + playerSizeX / 2
    local targetY
    if forcePosition == 1 then
        targetY = py - 70
    elseif forcePosition == 2 then
        targetY = py + 70
    else
        createMoveIntent(entity, 0, 0)
        return
    end
    local ex, ey = getEntityPosition(entity)
    local dx = targetX - ex
    local dy = targetY - ey
    local distance = math.sqrt(dx * dx + dy * dy)
    if distance > 5 then
        local speed = getEntitySpeed(entity)
        if distance > 0 then
            dx = (dx / distance) * speed
            dy = (dy / distance) * speed
        end
        createMoveIntent(entity, dx, dy)
    else
        createMoveIntent(entity, 0, 0)
    end
    createShootIntent(entity, 0)
end

function OnInteract(entity, interactorEntity)
    if isEntityPlayer(interactorEntity) and getParentId(entity) ~= interactorEntity then
        local forceCount = countForcesByType(interactorEntity, "flyingForce")
        if forceCount >= 2 then
            return
        end
        addPartId(interactorEntity, entity)
        setParentId(entity, interactorEntity)
        ex, _ = getEntityPosition(entity)
        ix, _ = getEntityPosition(interactorEntity)
        playerSizeX, playerSizeY = getEntitySize(getParentId(entity))
        entitySizeX, entitySizeY = getEntitySize(entity)
        isequipped = true
    end
end


