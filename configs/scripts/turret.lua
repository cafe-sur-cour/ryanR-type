function init(entity)
end

function update(entity, deltaTime)
    if createShootIntent(entity, 0.0) == true then
        setAnimationState(entity, "attack")
    end
end