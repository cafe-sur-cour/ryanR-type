function init(entity)
end

function update(entity, deltaTime)
end

function OnInteract(entity, interactorEntity)
    if isEntityPlayer(interactorEntity) then
        local currentSpeed = getEntitySpeed(interactorEntity)
        setEntitySpeed(interactorEntity, currentSpeed + 200.0)
        createDeathIntent(entity)
    end
end
