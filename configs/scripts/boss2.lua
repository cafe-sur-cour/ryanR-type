local bodyState = {
    headCreated = false,
    headId = 0,
    initialized = false
}

function init(entity)
    local gzX, gzY = getGameZonePosition()
    local gzWidth, gzHeight = getGameZoneSize()
    local bossWidth, bossHeight = getEntitySize(entity)
    local centerX = gzX + (gzWidth * 0.5) - 300
    local bottomY = gzHeight / 2
    setPosition(entity, centerX, bottomY)
    bodyState.initialized = true
    setGameZoneVelocity(0, 0)
end


function update(entity, dt)
    if not bodyState.initialized then
        init(entity)
    end

    if not bodyState.headCreated then
        local entityX, entityY = getEntityPosition(entity)
        local parts = getEntityParts(entity)
        if #parts == 0 then
            local gzX, gzY = getGameZonePosition()
            local gzWidth, gzHeight = getGameZoneSize()
            local headId = spawnEntity("boss2head", gzX + (gzWidth * 0.5), gzY + (gzHeight * 0.3))
            addPartId(entity, headId)
            setParentId(headId, entity)
            bodyState.headId = headId
        end
        bodyState.headCreated = true
    end
    createMoveIntent(entity, 0, 0)
end

function death(entity)
    local parts = getEntityParts(entity)
    for i, partId in ipairs(parts) do
        if isEntityAlive(partId) then
            createDeathIntent(partId, entity)
        end
    end
    setGameZoneVelocity(100, 0)
end
