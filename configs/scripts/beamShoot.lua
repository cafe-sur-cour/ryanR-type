offsetX = 0
offsetY = 0

function init(entity)
    createMoveIntent(entity, 0, 0)
    local ownerId = getOwner(entity)
    if ownerId ~= 0 then
        local px, py = getEntityPosition(ownerId)
        local ex, ey = getEntityPosition(entity)
        offsetX = ex - px
        offsetY = ey - py
    end
end

function update(entity, deltaTime)
    local ownerId = getOwner(entity)
    if ownerId ~= 0 then
        local px, py = getEntityPosition(ownerId)
        setPosition(entity, px + offsetX, py + offsetY)
    end
end

function death(entity)
    local px, py = getEntityPosition(entity)
    spawnEntity("beamShootDeath", px, py)
end