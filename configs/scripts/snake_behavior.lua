function init(entity)
    _movementTime = 0
    _segmentDistance = 50.0
    _segmentsCreated = false
    _attackCooldown = 0
    _attackDuration = 0
    _isAttacking = false
end

function update(entity, deltaTime)
    if not _segmentsCreated then
        local entityPositionx, entityPositiony = getEntityPosition(entity)
        local parts = getEntityParts(entity)
        if #parts == 0 then
            for i = 0, 6 do
                local segmentId = spawnEntity("snake_segment", entityPositionx - (i * 50), entityPositiony)
                addPartId(entity, segmentId)
                setParentId(segmentId, entity)
            end
        end
        _segmentsCreated = true
    end

    _movementTime = _movementTime + deltaTime
    _attackCooldown = _attackCooldown - deltaTime
    if _isAttacking then
        _attackDuration = _attackDuration - deltaTime
        if _attackDuration <= 0 then
            _isAttacking = false
            _attackCooldown = 3.0
        end
    end

    local px, py = getNearestPlayerPosition(entity)
    local hx, hy = getEntityPosition(entity)
    local dx = px - hx
    local dy = py - hy
    local dist = math.sqrt(dx*dx + dy*dy)
    local speed = getEntitySpeed(entity)
    local finalDirX = 0
    local finalDirY = 0
    finalDirX = 1.0
    if dist > 0 then
        local dirX = dx / dist
        local dirY = dy / dist

        if dist < 500 and _attackCooldown <= 0 then
            _isAttacking = true
            _attackDuration = 4.0
            _attackCooldown = 0
        end
        if _isAttacking then
            finalDirX = dirX
            finalDirY = dirY
            speed = speed * 1.5
        else
            local wave1 = math.sin(_movementTime * 2.0) * 0.4
            local wave2 = math.sin(_movementTime * 0.6) * 0.3
            local wave3 = math.cos(_movementTime * 1.4) * 0.2
            local perpX = -dirY
            local perpY = dirX
            local lateralStrength = 100.0
            local combinedWave = wave1 + wave2 + wave3
            finalDirX = 0.7 + perpX * combinedWave * lateralStrength * 0.01 + dirX * 0.15
            finalDirY = perpY * combinedWave * lateralStrength * 0.01 + dirY * 0.2
            local finalDist = math.sqrt(finalDirX*finalDirX + finalDirY*finalDirY)
            if finalDist > 0 then
                finalDirX = finalDirX / finalDist
                finalDirY = finalDirY / finalDist
            end
        end
    end
    if hy < 100 then
        finalDirY = 0.5
    elseif hy > 980 then
        finalDirY = -0.5
    end
    createMoveIntent(entity, finalDirX * speed, finalDirY * speed)
    local angle = math.atan(finalDirY, finalDirX) * 180 / math.pi
    setEntityRotation(entity, angle)

    local parts = getEntityParts(entity)
    local prevX, prevY = hx, hy
    local prevDirX, prevDirY = 0, 0

    for i, partId in ipairs(parts) do
        local currX, currY = getEntityPosition(partId)
        local targetX = prevX - prevDirX * _segmentDistance
        local targetY = prevY - prevDirY * _segmentDistance
        if i == 1 then
            targetX = hx - finalDirX * _segmentDistance
            targetY = hy - finalDirY * _segmentDistance
        end
        local tdx = targetX - currX
        local tdy = targetY - currY
        local tdist = math.sqrt(tdx*tdx + tdy*tdy)
        if tdist > 5 then
            local tspeed = getEntitySpeed(partId)
            if _isAttacking then
                tspeed = tspeed * 1.5
            end
            local tdirX = tdx / tdist
            local tdirY = tdy / tdist
            createMoveIntent(partId, tdirX * tspeed, tdirY * tspeed)
            local segmentAngle = math.atan(tdirY, tdirX) * 180 / math.pi
            setEntityRotation(partId, segmentAngle)
        end
        prevX, prevY = currX, currY
        if tdist > 0 then
            prevDirX = tdx / tdist
            prevDirY = tdy / tdist
        end
    end
end

function death(entity)
    local parts = getEntityParts(entity)
    for i, partId in ipairs(parts) do
        createDeathIntent(partId)
    end
end

function death(entity)
    local px, py = getEntityPosition(entity)
    local entitySizeX, entitySizeY = getEntitySize(entity)
    local explosionSizeX, explosionSizeY = getEntitySize("explodWaffle")
    px = px + entitySizeX / 2 - 566 / 2
    py = py + entitySizeY / 2 - 306 / 2
    spawnEntity("explodWaffle", px, py)
end
