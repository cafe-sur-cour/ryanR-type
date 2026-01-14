function init(entity)
end

function update(entity, deltaTime)
end

function OnInteract(entity, interactorEntity)
    if isEntityPlayer(interactorEntity) then
        local currentMaxCharge = getMaxCharge(interactorEntity)
        if currentMaxCharge < 5.0 then
            setMaxCharge(interactorEntity, currentMaxCharge + 0.5)
            createDeathIntent(entity)
        end
    end
end
