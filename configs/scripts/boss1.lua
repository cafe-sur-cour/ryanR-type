-- ============================================
-- BOSS 1 - Hilda Berg Style
-- ============================================

-- ============================================
-- Variables d'état du boss
-- ============================================
local bossState = {
    phase = 1,              -- Phase actuelle (1 ou 2)
    mode = "normal",        -- Mode: "normal", "charge"

    -- Système de comptage d'attaques pour changement de mode
    attackCount = 0,        -- Nombre d'attaques effectuées dans le mode actuel
    requiredAttacks = 5,    -- Nombre d'attaques requises avant changement de mode

    -- Timers
    attackTimer = 0,        -- Timer pour les attaques
    attackCooldown = 2.5,   -- Cooldown entre attaques

    -- Mouvement en 8
    figure8Timer = 0,       -- Timer pour le mouvement en 8
    figure8Speed = 1.2,     -- Vitesse du mouvement en 8

    -- Mode Charge
    chargeVerticalDir = 1,    -- Direction verticale (-1 ou 1)
    chargeCount = 0,    -- Nombre de charges effectuées
    maxCharges = 0,     -- Nombre total de charges à faire (1-2)
    chargeState = "moving",   -- "moving", "preparing", "charging", "returning"
    chargeTimer = 0,    -- Timer pour la charge
    chargeOscillation = 0,    -- Timer pour l'oscillation horizontale

    -- Position initiale
    initialized = false,
    startX = 0,
    startY = 0
}

-- ============================================
-- Fonction d'initialisation
-- ============================================
function init(entity)
    local currentX, currentY = getEntityPosition(entity)

    local gzX, gzY = getGameZonePosition()
    local gzWidth, gzHeight = getGameZoneSize()
    local bossWidth, bossHeight = getEntitySize(entity)

    if currentY < gzY or currentY > (gzY + gzHeight) then
        currentY = gzY + (gzHeight * 0.5)
    end
    if currentX < gzX or currentX > (gzX + gzWidth) then
        currentX = gzX + zWidth
    end
    setPosition(entity, currentX, currentY)
    bossState.startX = currentX
    bossState.startY = currentY
    bossState.initialized = true
    setGameZoneVelocity(0, 0)
end

-- ============================================
-- Mouvement en figure 8
-- ============================================
function moveInFigure8(entity, dt, centerX, centerY, amplitudeX, amplitudeY, speed)
    bossState.figure8Timer = bossState.figure8Timer + dt
    local gzX, gzY = getGameZonePosition()
    local gzWidth, gzHeight = getGameZoneSize()
    local bossWidth, bossHeight = getEntitySize(entity)
    local t = bossState.figure8Timer * bossState.figure8Speed
    local targetX = centerX + amplitudeX * math.sin(2 * t)
    local targetY = centerY + amplitudeY * math.sin(t)
    targetY = math.max(gzY + bossHeight * 0.5, math.min(targetY, gzY + gzHeight - bossHeight * 0.5))
    local currentX, currentY = getEntityPosition(entity)
    local dirX = (targetX - currentX)
    local dirY = (targetY - currentY)
    local dist = math.sqrt(dirX * dirX + dirY * dirY)
    if dist > 5 then
        dirX = (dirX / dist) * speed
        dirY = (dirY / dist) * speed
        createMoveIntent(entity, dirX, dirY)
    end
end

-- ============================================
-- Update principal
-- ============================================
function update(entity, dt)
    if not bossState.initialized then
        init(entity)
    end

    if bossState.phase == 1 then
        updatePhase1(entity, dt)
    elseif bossState.phase == 2 then
        updatePhase2(entity, dt)
    end
end

-- ============================================
-- Phase 1 - Trois modes
-- ============================================
function updatePhase1(entity, dt)
    -- Ne plus recalculer startX/startY - ils sont définis dans init()
    bossState.attackTimer = bossState.attackTimer + dt

    if bossState.mode == "normal" then
        updateNormalMode(entity, dt)
    elseif bossState.mode == "charge" then
        updateChargeMode(entity, dt)
    end

    if bossState.attackCount >= bossState.requiredAttacks then
        switchMode(entity)
    end
end

-- ============================================
-- Phase 2 - Placeholder
-- ============================================
function updatePhase2(entity, dt)
    -- TODO: Implémenter Phase 2
end

-- ============================================
-- Mode Normal - Mouvement en 8 vertical
-- ============================================
function updateNormalMode(entity, dt)
    -- Utiliser la position de spawn comme centre du mouvement en 8
    local centerX = bossState.startX
    local centerY = bossState.startY
    -- Amplitudes relatives à la taille de la game zone (pour la cohérence)
    local gzWidth, gzHeight = getGameZoneSize()
    local amplitudeX = gzWidth * 0.08
    local amplitudeY = gzHeight * 0.4
    local speed = 200

    moveInFigure8(entity, dt, centerX, centerY, amplitudeX, amplitudeY, speed)

    if bossState.attackTimer >= bossState.attackCooldown then
        local rand = math.random()
        if rand <= 0.7 then
            setAnimationState(entity, "shooting")
            shootAtNearestPlayer(entity)
        else
            spawnBlimpEnemies(entity)
            setAnimationState(entity, "summoning")
            spawnEntity("incante", getEntityPosition(entity))
        end
        bossState.attackTimer = 0
        bossState.attackCount = bossState.attackCount + 1
    end
end

-- ============================================
-- Mode Charge
-- ============================================
function updateChargeMode(entity, dt)
    local gzX, gzY = getGameZonePosition()
    local gzWidth, gzHeight = getGameZoneSize()
    local bossWidth, bossHeight = getEntitySize(entity)
    local currentX, currentY = getEntityPosition(entity)
    local centerX = bossState.startX
    local centerY = bossState.startY
    local rightQuarterWidth = gzWidth * 0.125
    if bossState.chargeState == "moving" then
        bossState.chargeOscillation = bossState.chargeOscillation + dt * 2
        local verticalSpeed = 180
        local targetY = centerY + (gzHeight * 0.15) * bossState.chargeVerticalDir
        targetY = math.max(gzY + bossHeight * 0.5, math.min(targetY, gzY + gzHeight - bossHeight * 0.5))

        if bossState.chargeVerticalDir == 1 and currentY >= targetY then
            bossState.chargeVerticalDir = -1
        elseif bossState.chargeVerticalDir == -1 and currentY <= targetY then
            bossState.chargeVerticalDir = 1
        end
        local oscillationX = centerX + math.sin(bossState.chargeOscillation) * (rightQuarterWidth * 0.5)
        local dirX = (oscillationX - currentX)
        local dirY = (targetY - currentY)
        local dist = math.sqrt(dirX * dirX + dirY * dirY)
        if dist > 5 then
            dirX = (dirX / dist) * verticalSpeed
            dirY = (dirY / dist) * verticalSpeed
            createMoveIntent(entity, dirX, dirY)
        end
        if bossState.attackTimer >= bossState.attackCooldown then
            if bossState.chargeCount == 0 then
                bossState.maxCharges = math.random(1, 2)
            end
            spawnEntity("inhale", currentX, currentY)
            bossState.chargeState = "preparing"
            bossState.chargeTimer = 0
            bossState.attackTimer = 0
            setAnimationState(entity, "dash1")
        end
    elseif bossState.chargeState == "preparing" then
        bossState.chargeTimer = bossState.chargeTimer + dt
        createMoveIntent(entity, 0, 0)
        if bossState.chargeTimer >= 1.0 then
            bossState.chargeState = "charging"
            bossState.chargeTimer = 0
            setAnimationState(entity, "dash2")
            spawnEntity("exhale", currentX, currentY)
        end
    elseif bossState.chargeState == "charging" then
        local targetX = gzX + (gzWidth * 0.15)
        local dirX = targetX - currentX
        local speed = 600
        if math.abs(dirX) > 10 then
            createMoveIntent(entity, (dirX / math.abs(dirX)) * speed, 0)
        else
            setAnimationState(entity, "idle")
            bossState.chargeState = "returning"
            bossState.chargeTimer = 0
        end
    elseif bossState.chargeState == "returning" then
        local targetX = centerX
        local dirX = targetX - currentX
        local speed = 400
        if math.abs(dirX) > 10 then
            createMoveIntent(entity, (dirX / math.abs(dirX)) * speed, 0)
        else
            bossState.chargeCount = bossState.chargeCount + 1
            if bossState.chargeCount >= bossState.maxCharges then
                bossState.attackCount = bossState.attackCount + 1
                bossState.chargeCount = 0
                bossState.maxCharges = 0
            end
            bossState.chargeState = "moving"
            bossState.attackTimer = 0
        end
    end
end

-- ============================================
-- Changement de mode ALEATOIRE
-- ============================================
function switchMode(entity)
    local modes = {"normal", "charge"}
    local availableModes = {}

    for _, mode in ipairs(modes) do
        if mode ~= bossState.mode then
            table.insert(availableModes, mode)
        end
    end

    local nextMode = availableModes[math.random(1, #availableModes)]


    bossState.mode = nextMode
    bossState.attackCount = 0
    bossState.attackTimer = 0
    bossState.figure8Timer = 0

    if nextMode == "charge" then
        bossState.requiredAttacks = 1
    else
        bossState.requiredAttacks = math.random(4, 7)
    end
    if bossState.mode == "charge" then
        bossState.chargeState = "moving"
        bossState.chargeCount = 0
        bossState.maxCharges = 0
        bossState.chargeVerticalDir = 1
        bossState.chargeOscillation = 0
    end
end

-- ============================================
-- Tirer vers le joueur le plus proche
-- ============================================
function shootAtNearestPlayer(entity)
    local ex, ey = getEntityPosition(entity)
    local px, py = getNearestPlayerPosition(entity)
    local dx = px - ex
    local dy = py - ey
    local angle_rad = math.atan(dy, dx)
    local angle_deg = angle_rad * 180 / math.pi
    createShootIntent(entity, angle_deg)
end

-- ============================================
-- Spawn d'ennemis Blimp avec positions aléatoires
-- ============================================
function spawnBlimpEnemies(entity)
    local currentX, currentY = getEntityPosition(entity)
    local gzX, gzY = getGameZonePosition()
    local gzWidth, gzHeight = getGameZoneSize()
    local enemyCount = math.random(1, 3)
    for i = 1, enemyCount do
        local prefabName = "greenBlimpEnemy"
        if math.random() > 0.7 then
            prefabName = "purpleBlimpEnemy"
        end
        local spawnX = currentX + math.random(-200, -10)
        local spawnY = gzY + math.random(100, gzHeight - 100)
        local enemyId = spawnEntity(prefabName, spawnX, spawnY)
    end
end

-- ============================================
-- Callback de mort
-- ============================================
function death(entity)
    setGameZoneVelocity(100, 0)
    local px, py = getEntityPosition(entity)
    local entitySizeX, entitySizeY = getEntitySize(entity)
    local explosionSizeX, explosionSizeY = getEntitySize("explode")
    px = px + entitySizeX / 2 - 298 / 2
    py = py + entitySizeY / 2 - 217 / 2
    spawnEntity("explode", px, py)
end