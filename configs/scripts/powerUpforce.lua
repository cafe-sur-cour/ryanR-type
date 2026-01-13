function init(entity)
end

function update(entity, deltaTime)
end

function OnInteract(entity, interactorEntity)
    if isEntityPlayer(interactorEntity) then
        local parts = getEntityParts(interactorEntity)
        addForceLevel(interactorEntity)
        createDeathIntent(entity)
    end
end