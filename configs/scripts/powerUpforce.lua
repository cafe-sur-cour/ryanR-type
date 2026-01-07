function init(entity)
end

function update(entity, deltaTime)
end

function OnInteract(entity, interactorEntity)
    if isEntityPlayer(interactorEntity) then
        addForceLevel(interactorEntity)
        createDeathIntent(entity)
    end
end