function init(entity)
    _G["time_" .. entity] = 0
    _G["spawn_timer_" .. entity] = 0
    _G["phase_" .. entity] = math.random() * 2 * math.pi
    _G["frequency_" .. entity] = 0.5 + math.random() * 1.5  -- random between 0.5 and 2.0
    _G["amp_factor_" .. entity] = 0.5 + math.random() * 0.5  -- random between 0.5 and 1.0
    local states = {"pink", "orange", "red", "orangeLight"}
    local randomIndex = math.random(1, #states)
    setAnimationState(entity, states[randomIndex])
end

function update(entity, deltaTime)
    _G["time_" .. entity] = (_G["time_" .. entity] or 0) + deltaTime
    _G["spawn_timer_" .. entity] = (_G["spawn_timer_" .. entity] or 0) + deltaTime

    local gzX, gzY = getGameZonePosition()
    local gzWidth, gzHeight = getGameZoneSize()
    local px, py = getEntityPosition(entity)
    local speed = getEntitySpeed(entity)

    -- Random zigzag movement
    local time = _G["time_" .. entity]
    local phase = _G["phase_" .. entity]
    local frequency = _G["frequency_" .. entity]
    local amplitude = (gzHeight - 100) * _G["amp_factor_" .. entity]
    local centerY = gzY + gzHeight / 2
    local oscillation = math.sin(time * frequency + phase) * amplitude + math.sin(time * frequency * 2 + phase) * (amplitude * 0.3)
    local targetY = centerY + oscillation

    if targetY < gzY + 50 then targetY = gzY + 50 end
    if targetY > gzY + gzHeight - 100 then targetY = gzY + gzHeight - 100 end

    local dirX = speed
    local dirY = (targetY - py) * 3

    createMoveIntent(entity, dirX, dirY)

    if _G["spawn_timer_" .. entity] > 0.5 and py > gzY + 60 and py < gzY + gzHeight - 110 then
        local randomIndex = math.random()

        if randomIndex <= 0.7 then
            spawnEntity("bubble", px - 30, py)
        else
            spawnEntity("starfish", px - 30, py)
        end
        _G["spawn_timer_" .. entity] = 0
    end
end

function death(entity)
end