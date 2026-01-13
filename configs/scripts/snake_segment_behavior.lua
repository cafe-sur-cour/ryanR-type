function init(entity)
end

function update(entity, deltaTime)
end

function death(entity)
    local parentId = getParentId(entity)
    if parentId ~= 0 then
        local parts = getEntityParts(parentId)
        local myIndex = -1
        for i, partId in ipairs(parts) do
            if partId == entity then
                myIndex = i
                break
            end
        end
        if myIndex > 0 then
            removePartId(parentId, entity)
            local currentParts = getEntityParts(parentId)
            if myIndex <= #currentParts then
                local x, y = getEntityPosition(entity)
                local newHeadId = spawnEntity("snake_head", x, y)
                for i = myIndex, #currentParts do
                    local partId = currentParts[i]
                    if isEntityAlive(partId) then
                        addPartId(newHeadId, partId)
                        setParentId(partId, newHeadId)
                        removePartId(parentId, partId)
                    end
                end
            end
        end
    end
end

function death(entity)
    local px, py = getEntityPosition(entity)
    local entitySizeX, entitySizeY = getEntitySize(entity)
    local explosionSizeX, explosionSizeY = getEntitySize("explode")
    px = px + entitySizeX / 2 - 298 / 2
    py = py + entitySizeY / 2 - 217 / 2
    spawnEntity("explode", px, py)
end