local config = {}
local angle = 0
local attacking = false
local attackTimer = 0

function Init(params)
    config.orbitRadius = params.orbitRadius or 80
    config.orbitSpeed = params.orbitSpeed or 2
    config.orbitAroundPlayer = params.orbitAroundPlayer or false
    config.orbitCenterX = params.orbitCenterX or 0
    config.orbitCenterY = params.orbitCenterY or 0
    config.attackSpeed = 100
    config.attackInterval = 1

    angle = math.random() * math.pi * 2
    attackTimer = math.random() * config.attackInterval

end

function Update(dt, time, posX, posY)
    local playerX, playerY = GetPlayerPosition()
    local dist = Distance(posX, posY, playerX, playerY)

    attackTimer = attackTimer + dt

    if attackTimer >= config.attackInterval and dist > 30 then
        attacking = true
    end

    if attacking and dist < 30 then
        attacking = false
        attackTimer = 0
    end

    if attacking then
        local dx = playerX - posX
        local dy = playerY - posY
        local normX, normY = Normalize(dx, dy)

        local newX = posX + normX * config.attackSpeed * dt
        local newY = posY + normY * config.attackSpeed * dt

        return newX, newY
    end

    angle = angle + config.orbitSpeed * dt

    if angle > math.pi * 2 then
        angle = angle - math.pi * 2
    end

    local centerX, centerY

    if config.orbitAroundPlayer then
        centerX, centerY = playerX, playerY
    else
        centerX = config.orbitCenterX
        centerY = config.orbitCenterY
    end

    local newX = centerX + math.cos(angle) * config.orbitRadius
    local newY = centerY + math.sin(angle) * config.orbitRadius

    return newX, newY
end