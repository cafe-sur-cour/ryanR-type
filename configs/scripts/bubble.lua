function init(entity)
    local states = {"pink", "blue"}
    local randomIndex = math.random(1, #states)
    setAnimationState(entity, states[randomIndex])
end

function update(entity, deltaTime)
end

function death(entity)
end