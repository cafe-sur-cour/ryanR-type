function init(entity)
end

function update(entity, deltaTime)
end

function OnInteract(entity, interactorEntity)
    if isEntityPlayer(interactorEntity) then
        local currentSpeed = getEntitySpeed(interactorEntity)
        if (currentSpeed < 800) then
            setEntitySpeed(interactorEntity, currentSpeed + 100.0)
        end
        createDeathIntent(entity)
    end
end
