-- Prefabs: one function per kind of object. Running this file only DEFINES them, nothing is
-- spawned yet. C++ calls them (asteroid fragments, waves) and so does StartGame() at the bottom.
-- (This was ent_creation.lua, which spawned the level while it ran, so it could only run once.)

local SCREEN_W, SCREEN_H = GetScreenSize()


-- Asteroid "prefab". size: 3 = large, 2 = medium, 1 = small.
-- C++ (AsteroidSystem, WaveSystem) calls this function, so this is the ONLY place that says what
-- an asteroid is made of.
-- Three shapes per size; every new asteroid (also every fragment) picks one at random.
--   radius: the visible outline is ~113 / 60 / 39 px wide inside the 160 / 96 / 64 px sprite tiles;
--           the circle is ~0.92 of half that, because the outlines are jagged, not round
--   offset: where the outline's center is, relative to the tile center (the medium ones aren't centered)
-- (Measured from the PNG: the bounding box of the pixels that are at least 75% opaque.)
local ASTEROID_SIZES = {
    [3] = {
        { animation = "asteroid1",        radius = 52, offset = { x =  1, y = -2 } },
        { animation = "asteroid2",        radius = 51, offset = { x =  0, y = -1 } },
        { animation = "asteroid3",        radius = 52, offset = { x =  1, y =  0 } },
    },
    [2] = {
        { animation = "asteroid_medium",  radius = 27, offset = { x = -5, y =  1 } },
        { animation = "asteroid_medium2", radius = 27, offset = { x = -7, y = -1 } },
        { animation = "asteroid_medium3", radius = 27, offset = { x = -9, y =  1 } },
    },
    [1] = {
        { animation = "asteroid_small",   radius = 18, offset = { x =  0, y =  0 } },
        { animation = "asteroid_small2",  radius = 17, offset = { x =  0, y =  2 } },
        { animation = "asteroid_small3",  radius = 17, offset = { x = -3, y =  1 } },
    },
}

function SpawnAsteroid(x, y, vx, vy, size)

    local shapes = ASTEROID_SIZES[size]
    local s      = shapes[math.random(#shapes)]  -- #t = length of t; math.random(n) = 1..n

    SpawnEntity("Asteroid", "Enemies", true, {

        Asteroid = { size = size },

        Transform = {
            x = x,
            y = y,
            scale = 1.0
        },

        Physics = {
            velocity = { x=vx, y=vy },
            damping = 1.0,
            angularVelocity = math.random(-90, 90),  -- deg/s, so the fragments don't all spin alike
            angularAcceleration = 0.0,
            angularDamping = 1.0,
            mass = 1.0
        },

        -- Visuals
        Sprite = {}, -- Default fallback
        Animation = {
            current_state = "IDLE",
            animations = {
                ["IDLE"]  = s.animation,
            }
        },

        Collider = {

            layer = "ASTEROID",
            mask = {"PLAYER", "PLAYER_BULLET", "SAUCER", "SAUCER_BULLET"},  -- saucers crash into asteroids too

            radius = s.radius,
            offset = s.offset
        }
    })
end


-- 4 bullets, like the arcade machine, which had exactly 4 bullet slots in its memory. The ship only
-- fires when a slot is free (PlayerSystem looks for an inactive one), so the size of this pool IS the
-- "at most 4 bullets on screen" rule: no extra code needed.
-- (The pool once held 4000, and every system walked over all of them every tick.)
local BULLET_SLOTS = 4

function SpawnBullets()

    for i = 1,BULLET_SLOTS,1
    do
        SpawnEntity("Bullet", "Bullets", false, {

            BulletTag = {},

            Life = {
                lifeTime = 0.0
            },


            Transform = {
                x = 20 * i,
                y = 20,
                scale = 1.0
            },

            Physics = {
                velocity = { x=0, y=0 },
                damping = 1.0,
                angularVelocity = 0.0,
                angularAcceleration = 0.0,
                angularDamping = 1.0,
                mass = 1.0
            },

            -- Visuals
            Sprite = {}, -- Default fallback
            Animation = {
                current_state = "IDLE",
                animations = {
                    ["IDLE"]  = "bullet",
                }
            },


            Collider = {

                layer = "PLAYER_BULLET",
                mask = {"ASTEROID", "SAUCER"},

                radius = 4  -- the bright dot is 6 px; the rest of the 32 px sprite is glow
            }
        })
    end
end


function SpawnShip()

    SpawnEntity("Player", "Player", true, {

        Ship = {},

        Transform = {
            x = SCREEN_W / 2,  -- start in the middle, whatever the screen size
            y = SCREEN_H / 2,
            rotation = -90,    -- nose up (the sprite points right); PlayerSystem respawns it the same way
            scale = 1.0
        },

        Physics = {
            velocity = { x=0, y=0 },
            damping = 0.548,        -- fraction of velocity kept per second (= 0.995 per tick at 120 Hz)
            angularVelocity = 0.0,
            angularAcceleration = 0.0,
            angularDamping = 0.548, -- fraction of angular velocity kept per second
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

        -- The engine plays the sound of the current state: nothing while IDLE, "thrust" (looped)
        -- while the PlayerSystem sets RUN
        Sound = {
            current_state = "IDLE",
            is_looping = true,
            sounds = {
                ["RUN"] = "thrust"
            }
        },

        Collider = {

            layer = "PLAYER",
            mask = {"ASTEROID", "SAUCER", "SAUCER_BULLET"},

            -- The ship is a 53 x 35 px triangle; no circle fits it exactly. This one is moved a bit toward
            -- the nose, so the nose and the wing tips both stick out ~10 px: forgiving, like the arcade.
            radius = 20,
            offset = { x = 4, y = -1 }
        }
    })
end


-- A flying saucer, coming in from the side with left-right speed vx. C++ (SaucerSystem) decides
-- where, which one and when, and flies it; this says what a saucer is made of.
function SpawnSaucer(x, y, vx, small)

    SpawnEntity("Saucer", "Enemies", true, {

        Saucer = { small = small },

        Transform = {
            x = x,
            y = y,
            scale = small and 0.5 or 1.0  -- Lua's "a and b or c" = C++'s a ? b : c (when b isn't false/nil)
        },

        Physics = {
            velocity = { x=vx, y=0 }
        },

        Sprite = {},
        Animation = {
            current_state = "IDLE",
            animations = {
                ["IDLE"] = "saucer"
            }
        },

        -- Its sound loops as long as it flies (the SoundSystem stops it when the saucer is switched off)
        Sound = {
            current_state = "IDLE",
            is_looping = true,
            sounds = {
                ["IDLE"] = small and "saucerSmall" or "saucerBig"
            }
        },

        Collider = {

            layer = "SAUCER",
            mask = {"PLAYER", "PLAYER_BULLET", "ASTEROID"},  -- not its own shots

            -- The outline is 73 x 47 px; like the ship's, the circle lets the edges stick out a bit.
            -- Multiplied by the scale, so the small saucer's is half as big.
            radius = 28,
            offset = { x = 0, y = -2 }
        }
    })
end


-- The saucers' shots: 2 slots for all saucers, like the arcade. No BulletTag: that marks the
-- PLAYER's bullets, the ones that score.
function SpawnSaucerBullets()

    for i = 1,2,1
    do
        SpawnEntity("SaucerBullet", "SaucerBullets", false, {

            Life = {
                lifeTime = 0.0
            },

            Transform = {
                x = 0,
                y = 0,
                scale = 1.0
            },

            Physics = {
                velocity = { x=0, y=0 }
            },

            Sprite = {},
            Animation = {
                current_state = "IDLE",
                animations = {
                    ["IDLE"] = "bullet"
                }
            },

            Collider = {

                layer = "SAUCER_BULLET",
                mask = {"PLAYER", "ASTEROID"},

                radius = 4
            }
        })
    end
end


-- One speck of an explosion: flies off, slows down, and is destroyed when its time is up.
-- C++ (EffectsSystem) picks the directions, speeds and lifetimes.
function SpawnDebris(x, y, vx, vy, lifetime)

    SpawnEntity("Debris", "Effects", true, {

        Life = {
            lifeTime = lifetime,
            destroyOnExpire = true  -- not pooled: these would pile up forever if only switched off
        },

        Transform = {
            x = x,
            y = y,
            scale = 0.5
        },

        Physics = {
            velocity = { x=vx, y=vy },
            damping = 0.3  -- keeps only 30% of its speed per second
        },

        Sprite = {},
        Animation = {
            current_state = "IDLE",
            animations = {
                ["IDLE"] = "bullet"  -- the same glowing dot as a bullet, at half size
            }
        }
    })
end


-- A new game. C++ (Asteroids::startNewGame) has just emptied the Scene. No asteroids here: the
-- WaveSystem sees an empty sky and starts wave 1 by itself.
function StartGame()
    SpawnShip()
    SpawnBullets()
    SpawnSaucerBullets()
end
