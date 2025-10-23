local config = {}
local currentWaypoint = 1
local waypoints = {}

function Init(params)
    config.patrolSpeed = params.patrolSpeed or 40
    config.waypointThreshold = 10

    waypoints = params.waypoints or {}

    if #waypoints == 0 then
        waypoints = {
            {x = 100, y = 100},
            {x = 300, y = 100},
            {x = 300, y = 300},
            {x = 100, y = 300}
        }
    end

    print("Patrol Pattern initialized with " .. #waypoints .. " waypoints")
    print("  Speed: " .. config.patrolSpeed)
end

function Update(dt, time, posX, posY)
    if #waypoints == 0 then
        return posX, posY
    end

    local target = waypoints[currentWaypoint]

    local dist = Distance(posX, posY, target.x, target.y)

    if dist < config.waypointThreshold then
        currentWaypoint = currentWaypoint + 1
        if currentWaypoint > #waypoints then
            currentWaypoint = 1
        end
        target = waypoints[currentWaypoint]
    end

    local dx = target.x - posX
    local dy = target.y - posY

    local normX, normY = Normalize(dx, dy)

    local newX = posX + normX * config.patrolSpeed * dt
    local newY = posY + normY * config.patrolSpeed * dt

    return newX, newY
end