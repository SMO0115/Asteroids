print("[Lua] Initializing Level 1 from C++ Definition...")




SpawnEntity("Asteroid", "Enemies", {
    Transform = {
        x = 400,
        y = 750,
        scale = 1.0
    },


    Physics = {
        velocity = { x=-100, y=-50 },
        damping = 1.0,
        angularVelocity = 50.0,
        angularAcceleration = 0.0,
        angularDamping = 1.0,
        mass = 1.0
    },

    -- Visuals
    Sprite = {}, -- Default fallback
    Animation = {
        current_state = "IDLE",
        animations = {
            ["IDLE"]  = "asteroid1",
        }
    },
})



SpawnEntity("Asteroid", "Enemies", {
    Transform = {
        x = 200,
        y = 150,
        scale = 1.0
    },

    Physics = {
        velocity = { x=-100, y=-20 },
        damping = 1.0,
        angularVelocity = 50.0,
        angularAcceleration = 0.0,
        angularDamping = 1.0,
        mass = 1.0
    },

    -- Visuals
    Sprite = {}, -- Default fallback
    Animation = {
        current_state = "IDLE",
        animations = {
            ["IDLE"]  = "asteroid1",
        }
    },
})



SpawnEntity("Asteroid", "Player", {
    Transform = {
        x = 400,
        y = 400,
        scale = 1.0
    },

    Physics = {
        velocity = { x=0, y=0 },
        damping = 0.995,
        angularVelocity = 0.0,
        angularAcceleration = 0.0,
        angularDamping = 0.995,
        mass = 1.0
    },

    -- Visuals
    Sprite = {}, -- Default fallback
    Animation = {
        current_state = "IDLE",
        animations = {
            ["IDLE"]  = "player_idle",
            ["RUN" ]  = "player_run"
        }
    },
})