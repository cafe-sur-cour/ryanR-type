function init(entity)
end

function update(entity, deltaTime)
end

function OnInteract(entity, interactorEntity)
    if isEntityPlayer(interactorEntity) then
        local currentReloadTime = getChargeReloadTime(interactorEntity)
        if currentReloadTime > 0.1 then
            setChargeReloadTime(interactorEntity, math.max(currentReloadTime * 0.9, 0.1))
            createDeathIntent(entity)
        end
    end
end
