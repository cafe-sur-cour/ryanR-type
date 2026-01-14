function init(entity)
end

function update(entity, dt)
    local vx, vy = getEntityVelocity(entity)
    if vx ~= 0 or vy ~= 0 then
        local angle = math.atan(vy, vx) * 180 / math.pi
        setEntityRotation(entity, angle)
    end
end
