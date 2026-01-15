function init(entitu)
end

function update(entity, deltaTime)
end

function death(entity)
    local px, py = getEntityPosition(entity)
    local entitySizeX, entitySizeY = getEntitySize(entity)
    px = px + entitySizeX / 2 - 74 / 2
    py = py + entitySizeY / 2 - 66 / 2
    spawnEntity("explosionBomb", px, py)
end