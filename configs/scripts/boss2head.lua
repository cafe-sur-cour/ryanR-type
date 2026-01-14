local headState = {
    movementTime = 0,
    targetX = 0,
    targetY = 0,
    reachedTarget = false,
    attackTimer = 0,
    attackCooldown = 1.5,
    isVulnerable = false,
    vulnerableTimer = 0,
    vulnerableDuration = 5.0,
    invulnerableTimer = 0,
    isTeleporting = false,
    teleportTimer = 0,
    teleportDelay = 0,
    teleportCooldown = 0,
    isHitReturning = false,
    hitReturnTimer = 0,
    initialized = false,
    hitCooldown = 0,
    hasShoot = false
}

function init(entity)
    local gzX, gzY = getGameZonePosition()
    local gzWidth, gzHeight = getGameZoneSize()
    pickNewTarget(entity)
    headState.initialized = true
    headState.invulnerableTimer = 8.0
    headState.isVulnerable = false
    headState.teleportCooldown = 5.0
    headState.hitCooldown = 0
    setInvulnerable(entity, true)
end

function pickNewTarget(entity)
    local gzX, gzY = getGameZonePosition()
    local gzWidth, gzHeight = getGameZoneSize()
    local bossWidth, bossHeight = getEntitySize(entity)
    headState.targetX = gzX + math.random(100, gzWidth - 100)
    headState.targetY = gzY + math.random(100, gzHeight - 300)
    headState.reachedTarget = false
end

function update(entity, dt)
    if not headState.initialized then
        init(entity)
    end

    if headState.hitCooldown > 0 then
        headState.hitCooldown = headState.hitCooldown - dt
    end

    if headState.isHitReturning then
        headState.hitReturnTimer = headState.hitReturnTimer + dt
        createMoveIntent(entity, 0, 0)

        if headState.hitReturnTimer >= 0.5 then
            headState.isHitReturning = false
            headState.isTeleporting = true
            headState.teleportTimer = 0
            headState.teleportDelay = math.random(2, 3)
            setAnimationState(entity, "disappear")
        end
        return
    end

    if headState.isTeleporting then
        headState.teleportTimer = headState.teleportTimer + dt
        createMoveIntent(entity, 0, 0)
        if headState.teleportTimer >= headState.teleportDelay then
            pickNewTarget(entity)
            setPosition(entity, headState.targetX, headState.targetY)
            headState.isTeleporting = false
            headState.teleportTimer = 0
            headState.isVulnerable = false
            headState.invulnerableTimer = math.random(8, 12)
            setAnimationState(entity, "appear")
            setInvulnerable(entity, true)
        end
        return
    end

    if headState.isVulnerable then
        headState.vulnerableTimer = headState.vulnerableTimer + dt
        if headState.vulnerableTimer >= headState.vulnerableDuration then
            headState.vulnerableTimer = 0
            headState.invulnerableTimer = math.random(8, 15)
            headState.isVulnerable = false
            setAnimationState(entity, "return")
            setInvulnerable(entity, true)
            headState.hitCooldown = 1.0
        end
    else
        headState.invulnerableTimer = headState.invulnerableTimer - dt
        if headState.invulnerableTimer <= 0 then
            headState.vulnerableDuration = math.random(4, 7)
            headState.isVulnerable = true
            headState.vulnerableTimer = 0
            setAnimationState(entity, "wakeup")
            setInvulnerable(entity, false)
            headState.hitCooldown = 1.0
        end
        headState.teleportCooldown = headState.teleportCooldown - dt
        if headState.teleportCooldown <= 0 then
            if math.random() < 0.3 then
                setAnimationState(entity, "disappear")
                headState.isTeleporting = true
                headState.teleportTimer = 0
                headState.teleportDelay = math.random(1, 2)
            end
            headState.teleportCooldown = math.random(4, 8)
        end
    end

    if headState.isTeleporting or headState.isHitReturning then
        return
    end

    headState.movementTime = headState.movementTime + dt
    local currentX, currentY = getEntityPosition(entity)

    local dx = headState.targetX - currentX
    local dy = headState.targetY - currentY
    local dist = math.sqrt(dx*dx + dy*dy)

    if dist < 50 or headState.reachedTarget then
        pickNewTarget(entity)
        dx = headState.targetX - currentX
        dy = headState.targetY - currentY
        dist = math.sqrt(dx*dx + dy*dy)
    end

    local speed = getEntitySpeed(entity)
    if dist > 5 then
        local dirX = dx / dist
        local dirY = dy / dist
        createMoveIntent(entity, dirX * speed, dirY * speed)
    else
        headState.reachedTarget = true
    end

    headState.attackTimer = headState.attackTimer + dt

    if headState.hasShoot then
        setProjectilePrefab(entity, "fireShoot")
        createShootIntent(entity, 45)
        headState.hasShoot = false
    end

    if headState.attackTimer >= headState.attackCooldown then
        if headState.isVulnerable then
            setAnimationState(entity, "ShootingPink")
        else
            setAnimationState(entity, "shootingNormal")
        end
        setProjectilePrefab(entity, "bellShoot")
        createShootIntent(entity, 0)
        headState.hasShoot = true
        headState.attackTimer = 0
        headState.attackCooldown = math.random(1, 3)
    end
end

function OnInteract(entity, interactorEntity)
    if headState.isVulnerable then
        return false
    end
    return false
end

function death(entity)
    local parentId = getParentId(entity)
    if parentId ~= 0 and isEntityAlive(parentId) then
        createDeathIntent(parentId, entity)
    end
end
