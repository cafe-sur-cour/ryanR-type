function init(entitu)
end

function update(entity, deltaTime)
end

function death(entity)
    local px, py = getEntityPosition(entity)
    local entitySizeX, entitySizeY = getEntitySize(entity)
    px = px + entitySizeX / 2 - 74 / 2
    py = py + entitySizeY / 2 - 66 / 2
    local explosion = spawnEntity("explosionBomb", px, py)
    local owner = getEntityOwner(entity)
    if owner ~= 0 then
        setEntityOwner(explosion, owner)
    end
end