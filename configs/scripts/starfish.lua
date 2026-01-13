function init(entity)
    local states = {"pink", "orange", "yellow"}
    local randomIndex = math.random(1, #states)
    setAnimationState(entity, states[randomIndex])
end

function update(entity, deltaTime)
end

function death(entity)
    local px, py = getEntityPosition(entity)
    spawnEntity("explosion", px, py)
end