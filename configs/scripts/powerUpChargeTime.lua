function init(entity)
end

function update(entity, deltaTime)
end

function OnInteract(entity, interactorEntity)
    if isEntityPlayer(interactorEntity) then
        local currentMaxCharge = getMaxCharge(interactorEntity)
        if currentMaxCharge > 0.6 then
            setMaxCharge(interactorEntity, currentMaxCharge - 0.2)
            createDeathIntent(entity)
        end
    end
end
