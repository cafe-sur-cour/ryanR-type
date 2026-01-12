function init(entity)
end

function update(entity, deltaTime)
end

function OnInteract(entity, interactorEntity)
    if isEntityPlayer(interactorEntity) then
        print("[powerUpforce] Player picked up power-up, adding force level")
        local parts = getEntityParts(interactorEntity)
        print("[powerUpforce] Player entity ID: " .. interactorEntity .. " has " .. #parts .. " parts")
        for i, partId in ipairs(parts) do
            print("[powerUpforce] Part " .. i .. ": entity ID " .. partId)
        end
        addForceLevel(interactorEntity)
        createDeathIntent(entity)
    end
end