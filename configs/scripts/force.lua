level = 1
isequipped = false
attachDir = 0
playerSizeX = 0
playerSizeY = 0
entitySizeX = 0
entitySizeY = 0
targetX = 0
targetY = 0
isMovingToTarget = false

function init(entity)
end

function update(entity, deltaTime)
    if (getParentId(entity) == 0 or attachDir == 0) then
        if isMovingToTarget then
            local ex, ey = getEntityPosition(entity)
            local dx = targetX - ex
            local dy = targetY - ey
            local distance = math.sqrt(dx * dx + dy * dy)
            if distance > 10 then
                local speed = getEntitySpeed(entity)
                local dirX = dx / distance * speed
                local dirY = dy / distance * speed
                createMoveIntent(entity, dirX, dirY)
            else
                createMoveIntent(entity, 0, 0)
                isMovingToTarget = false
            end
        end
        return
    end
    px, py = getEntityPosition(getParentId(entity))
    ex, ey = getEntityPosition(entity)

    if( attachDir == -1 ) then
        px = px - entitySizeX - 20
    else
        px = px + playerSizeX + 20
    end
    setPosition(entity, px, py)
end

function OnInteract(entity, interactorEntity)
    if isEntityPlayer(interactorEntity) and getParentId(entity) ~= interactorEntity then
        local forceCount = countForcesByType(interactorEntity, "force")
        if forceCount >= 1 then
            print("[force] Already 1 force attached, rejecting")
            return
        end


        addPartId(interactorEntity, entity)
        setParentId(entity, interactorEntity)
        ex, _ = getEntityPosition(entity)
        ix, _ = getEntityPosition(interactorEntity)
        playerSizeX, playerSizeY = getEntitySize(getParentId(entity))
        entitySizeX, entitySizeY = getEntitySize(entity)
        if ex > ix then
            attachDir = 1
        else
            attachDir = -1
        end
        isequipped = true

        if level == 2 then
            setProjectilePrefab(interactorEntity, "heavy_shot")
        end
        if (level >= 3) then
            setProjectilePrefab(interactorEntity, "triple_shot")
        end
    end
end

function ActivateOrDeactivateForce(entity, entityCaller)
    if isequipped == false then
        targetX, targetY = getEntityPosition(entityCaller)
        isMovingToTarget = true
    else
        isequipped = false
        removePartId(getParentId(entity), entity)
        setProjectilePrefab(getParentId(entity), "basic_shot")
        setParentId(entity, 0)
        attachDir = 0
        isMovingToTarget = false
        createMoveIntent(entity, 0, 0)
    end
end

function addLevel(entity)
    level = level + 1
    print("[force] New force level: " .. level)
    if (level > 3) then
        level = 3
    end
    local parentId = getParentId(entity)
    if isEntityPlayer(parentId) == false then
        print("[force] Parent is not player, not changing projectile. Parent ID: " .. parentId)
        return
    end
    if level == 2 then
        setProjectilePrefab(parentId, "heavy_shot")
        print("[force] Setting heavy shot")
        setAnimationState(entity, "level2")
        print("[force] Setting animation to level2")
    end
    if (level == 3) then
        setAnimationState(entity, "level3")
        setProjectilePrefab(parentId, "triple_shot")
    end
end