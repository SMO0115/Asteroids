print("[Lua] Starting Asset Loading...")

local entities = {
    "invader_A",
    "invader_B",
    "invader_C",
    "player",
    "UFO",
    "player_death",
    "invader_death"
}

for _, name in ipairs(entities) do
    loadTexture(name, "assets/Sprites/" .. name .. ".png")
    loadAnimation(name, "assets/Sprites/" .. name .. ".json")
end


for k = 0, 3 do
    for i = 0, 3 do
        for j = 0, 3 do
            local suffix = k .. "_" .. i .. "_" .. j

            local wall_id = "wall_" .. suffix
            loadTexture(wall_id, "assets/Sprites/wall.png")
            loadAnimation(wall_id, "assets/Sprites/wall.json")

            local dest_id = "wall_destruction_" .. suffix
            loadTexture(dest_id, "assets/Sprites/destruction.png")
            loadAnimation(dest_id, "assets/Sprites/invader_death.json")
        end
    end
end


local projectiles = {
    "missile",
    "projectile_A",
    "projectile_B",
    "projectile_C",
    "player_missile"
}

for _, name in ipairs(projectiles) do
    loadTexture(name, "assets/Sprites/" .. name .. ".png")
    loadAnimation(name, "assets/Sprites/" .. name .. ".json")
end


loadSound("explosion",     "assets/Audio/explosion.wav")
loadSound("player_shoot",  "assets/Audio/shoot.wav")
loadSound("shoot",         "assets/Audio/shoot.wav")
loadSound("ufo_highpitch", "assets/Audio/ufo_highpitch.wav")
loadSound("ufo_lowpitch",  "assets/Audio/ufo_lowpitch.wav")


for i = 1, 4 do
    loadSound("fastinvader" .. i, "assets/Audio/fastinvader" .. i .. ".wav")
end


loadFont("pixel", "assets/Fonts/PixelifySans.ttf")

print("[Lua] Assets loaded successfully.")