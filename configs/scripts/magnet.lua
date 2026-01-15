function init(entity)
end

function update(entity, deltaTime)
    local posEnemyX, posEnemyY = getNearestEnemyPosition(entity)
    if posEnemyX == 0 and posEnemyY == 0 then
        return
    end
    local posMagnetX, posMagnetY = getEntityPosition(entity)
    local dirX = posEnemyX - posMagnetX
    local dirY = posEnemyY - posMagnetY
    local entitySpeed = getEntitySpeed(entity)
    local length = math.sqrt(dirX * dirX + dirY * dirY)
    if length > 0 and length then
        dirX = dirX / length
        dirY = dirY / length
        createMoveIntent(entity, dirX * entitySpeed, dirY * entitySpeed)
    end
end
