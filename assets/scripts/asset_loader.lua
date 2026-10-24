

loadTexture("asteroids", "assets/Sprites/asteroids.png")

-- Three shapes per asteroid size (the three columns of the sprite sheet)
loadAnimation("asteroid1", "assets/Sprites/asteroid1.json")                -- large  (160 px)
loadAnimation("asteroid2", "assets/Sprites/asteroid2.json")
loadAnimation("asteroid3", "assets/Sprites/asteroid3.json")
loadAnimation("asteroid_medium", "assets/Sprites/asteroid_medium.json")    -- medium (96 px)
loadAnimation("asteroid_medium2", "assets/Sprites/asteroid_medium2.json")
loadAnimation("asteroid_medium3", "assets/Sprites/asteroid_medium3.json")
loadAnimation("asteroid_small", "assets/Sprites/asteroid_small.json")      -- small  (64 px)
loadAnimation("asteroid_small2", "assets/Sprites/asteroid_small2.json")
loadAnimation("asteroid_small3", "assets/Sprites/asteroid_small3.json")
loadAnimation("player_idle", "assets/Sprites/player_idle.json")
loadAnimation("player_run", "assets/Sprites/player_run.json")
loadAnimation("bullet", "assets/Sprites/bullet.json")

loadFont("pixel", "assets/Fonts/PixelifySans.ttf", 72)                   -- big labels ("PAUSED")
loadFont("pixel", "assets/Fonts/PixelifySans.ttf", 32)                   -- HUD (score)

-- The arcade's sounds. C++ plays them by these names (PlaySoundEvent), the ship's Sound component too.
loadSound("fire",       "assets/sounds/fire/fire.wav")
loadSound("thrust",     "assets/sounds/thrust/thrust.wav")          -- loops while UP is held
loadSound("bangLarge",  "assets/sounds/banglarge/bangLarge.wav")
loadSound("bangMedium", "assets/sounds/bangmedium/bangMedium.wav")
loadSound("bangSmall",  "assets/sounds/bangsmall/bangSmall.wav")
loadSound("extraShip",  "assets/sounds/extraship/extraShip.wav")
loadSound("beat1",      "assets/sounds/beat1/beat1.wav")            -- the "heartbeat": beat1, beat2, beat1, ...
loadSound("beat2",      "assets/sounds/beat2/beat2.wav")
