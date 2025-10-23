local config = {}

function Init(params)
    config.speed = params.speed or 60
    config.trackingDistance = params.trackingDistance or 300
    print("Tracking Pattern initialized with speed: " .. config.speed)
end

function Update(dt, time, posX, posY)
    -- posición del jugador
    local playerX, playerY = GetPlayerPosition()

    -- distancia al jugador
    local dist = Distance(posX, posY, playerX, playerY)

    if dist < config.trackingDistance and dist > 5 then
        local dx = playerX - posX
        local dy = playerY - posY

        local normX, normY = Normalize(dx, dy)

        local newX = posX + normX * config.speed * dt
        local newY = posY + normY * config.speed * dt

        return newX, newY
    end

    return posX, posY
end