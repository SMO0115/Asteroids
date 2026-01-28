//
// Created by marcel on 1/24/26.
//

#include <unordered_map>
#include <iostream>

#include "engine/modules/physics/PhysicsComponent.h"
#include "engine/modules/graphics/AnimationComponent.h"
#include "engine/modules/graphics/SpriteComponent.h"
#include "engine/modules/graphics/UITextComponent.h"
#include "engine/modules/audio/SoundComponent.h"
#include "engine/core/components/TransformComponent.h"
#include "engine/modules/physics/ColliderComponent.h"

#include "engine/scripting/ComponentRegistry.h"
#include "engine/scripting/StateRegistry.h"
#include "engine/core/GameObject.h"

#include "game/core/GameTypes.h"

using namespace Engine::Scripting;

// =================================================================================
// 1. TRANSFORM
// =================================================================================
static bool s_transform_registered = ComponentRegistry::Get().registerComponent(
    "Transform",
    [](Engine::Core::GameObject& obj, const sol::table& t) {

        auto& transform = obj.addComponent<Engine::Core::TransformComponent>();
        transform.position = glm::vec2(t.get_or("x", 0), t.get_or("y", 0));
        transform.scale    = t.get_or("scale", 1.f);
        transform.rotation = t.get_or("rotation", 0.f);
    }
);

// =================================================================================
// 2. SOUND (UPDATED)
// =================================================================================
static bool s_sound_registered = ComponentRegistry::Get().registerComponent(
    "Sound",
    [](Engine::Core::GameObject& obj, const sol::table& t) {

        auto& sound = obj.addComponent<Engine::Audio::SoundComponent>();
        sound.is_looping = t.get_or("is_looping", false);

        // A. Set Current State (String -> Int)
        // Lua: current_state = "IDLE"
        std::string stateStr = t.get_or("current_state", std::string("IDLE"));
        sound.current_state  = StateRegistry::Get().getState("SoundState", stateStr);

        // B. Load Sound Map (String Key -> Int Key)
        // Lua: sounds = { IDLE = "assets/sounds/idle.wav", WALK = "..." }
        if (sol::optional<sol::table> sounds_table = t["sounds"]) {
            for (const auto& kv : *sounds_table) {
                // Key is String ("IDLE"), Value is String ("path")
                if (kv.first.is<std::string>() && kv.second.is<std::string>()) {

                    std::string keyState = kv.first.as<std::string>();
                    std::string assetPath = kv.second.as<std::string>();

                    // Resolve "IDLE" -> 0
                    int stateID = StateRegistry::Get().getState("SoundState", keyState);

                    // Store: [0] = "assets/sounds/idle.wav"
                    sound.sounds[stateID] = assetPath;
                }
            }
        }
    }
);

// =================================================================================
// 3. COLLIDER
// =================================================================================
static bool s_collider_registered = Engine::Scripting::ComponentRegistry::Get().registerComponent(
    "Collider",
    [](Engine::Core::GameObject& obj, const sol::table& t) {

        auto& collider = obj.addComponent<Engine::Physics::ColliderComponent>();

        // 1. Bounds
        if (sol::optional<sol::table> bounds = t["bounds"]) {
            collider.bounds.x = bounds->get_or("x", 0.f);
            collider.bounds.y = bounds->get_or("y", 0.f);
            collider.bounds.w = bounds->get_or("w", 0.f);
            collider.bounds.h = bounds->get_or("h", 0.f);
        }

        // 2. Helper: Parse Mask using StateRegistry
        auto parseMask = [](const sol::object& input) -> int {
            int mask = 0;

            // Helper to lookup ONE string in the registry
            auto resolve = [](const std::string& key) {
                return Engine::Scripting::StateRegistry::Get().getState("CollisionLayer", key);
            };

            // Case A: Single String ("PLAYER")
            if (input.is<std::string>()) {
                mask = resolve(input.as<std::string>());
            }
            // Case B: List of Strings ({"WALL", "INVADER"})
            else if (input.is<sol::table>()) {
                for (const auto& pair : input.as<sol::table>()) {
                    if (pair.second.is<std::string>()) {
                        // Bitwise OR the values together
                        mask |= resolve(pair.second.as<std::string>());
                    }
                }
            }
            return mask;
        };

        // 3. Assign (Cast int back to Enum)
        using LayerType = Engine::Physics::CollisionLayer;

        if (t["layer"].valid())
            collider.layer = static_cast<LayerType>(parseMask(t["layer"]));

        if (t["mask"].valid())
            collider.mask  = static_cast<LayerType>(parseMask(t["mask"]));
    }
);


// =================================================================================
// 4. PHYSICS
// =================================================================================
static bool s_physics_registered = ComponentRegistry::Get().registerComponent(
    "Physics",
    [](Engine::Core::GameObject& obj, const sol::table& t) {

        auto& phys = obj.addComponent<Engine::Physics::PhysicsComponent>();
        phys.mass = t.get_or("mass", 1.0f);

        auto getVec2 = [&](const std::string& key) -> glm::vec2 {
            sol::optional<sol::table> vec = t[key];
            if (vec) return glm::vec2(vec->get_or("x", 0.f), vec->get_or("y", 0.f));
            return glm::vec2(0.f);
        };

        phys.velocity        = getVec2("velocity");
        phys.acceleration    = getVec2("acceleration");
        phys.angularVelocity = getVec2("angularVelocity"); // Assuming your struct uses vec2
        phys.angularDamping  = getVec2("angularDamping");
    }
);

// =================================================================================
// 5. ANIMATION (UPDATED)
// =================================================================================
static bool s_animation_registered = ComponentRegistry::Get().registerComponent(
    "Animation",
    [](Engine::Core::GameObject& obj, const sol::table& t) {

        auto& anim = obj.addComponent<Engine::Graphics::AnimationComponent>();
        anim.speed = t.get_or("speed", 1.0f);

        // A. Set Initial State (String -> Int)
        std::string curState = t.get_or("current_state", std::string("IDLE"));
        anim.current_state = StateRegistry::Get().getState("AnimationState", curState);

        // B. Load Animations Map (String Key -> Int Key)
        // Lua: animations = { IDLE = "anim_player_idle.json", RUN = "..." }
        sol::optional<sol::table> anim_table = t["animations"];
        if (anim_table) {
            for (const auto& kv : *anim_table) {
                if (kv.first.is<std::string>() && kv.second.is<std::string>()) {

                    std::string stateName = kv.first.as<std::string>();
                    std::string assetId   = kv.second.as<std::string>();

                    // Resolve "IDLE" -> 0
                    int stateID = StateRegistry::Get().getState("AnimationState", stateName);

                    // Note: Ensure your AnimationComponent has a map that accepts size_t/int keys
                    // e.g., std::unordered_map<size_t, std::string> animation_paths;
                    anim.animations[stateID] = assetId;
                }
            }
        }
    }
);

// =================================================================================
// 6. SPRITE
// =================================================================================
static bool s_sprite_registered = ComponentRegistry::Get().registerComponent(
    "Sprite",
    [](Engine::Core::GameObject& obj, const sol::table& t) {

        auto& sprite = obj.addComponent<Engine::Graphics::SpriteComponent>();

        // Lua string ("player.png") is stored directly to be resolved later
        // sprite.texture = t.get_or("texture", std::string(""));

        if (sol::optional<sol::table> rect = t["rect"]) {
            sprite.sourceRect.x = rect->get_or("x", 0);
            sprite.sourceRect.y = rect->get_or("y", 0);
            sprite.sourceRect.w = rect->get_or("w", 0);
            sprite.sourceRect.h = rect->get_or("h", 0);
        }

        if (sol::optional<sol::table> col = t["color"]) {
            sprite.color.r = col->get_or("r", 255);
            sprite.color.g = col->get_or("g", 255);
            sprite.color.b = col->get_or("b", 255);
            sprite.color.a = col->get_or("a", 255);
        }
    }
);

// =================================================================================
// 7. UI TEXT
// =================================================================================
static bool s_uitext_registered = ComponentRegistry::Get().registerComponent(
    "UIText",
    [](Engine::Core::GameObject& obj, const sol::table& t) {

        auto& ui = obj.addComponent<Engine::Graphics::UITextComponent>();

        ui.font_name = t.get_or("font_name", std::string("default_font"));
        ui.text      = t.get_or("text", std::string("New Text"));
        ui.size      = t.get_or("size", 12);
        ui.is_dirty  = true;

        ui.x_offset = t.get_or("x_offset", 0);
        ui.y_offset = t.get_or("y_offset", 0);

        if (sol::optional<sol::table> col = t["color"]) {
            ui.color.r = col->get_or("r", 255);
            ui.color.g = col->get_or("g", 255);
            ui.color.b = col->get_or("b", 255);
            ui.color.a = col->get_or("a", 255);
        }
    }
);

// =================================================================================
// 8. STATE REGISTRATION (Game Specifics)
// =================================================================================

// --- ANIMATION STATES ---
// Maps Lua Strings (from .json or .lua) to C++ Enums
static bool s_anim_states = StateRegistry::Get().registerBatch("AnimationState", {
    // Basic States
    { "IDLE" ,      (int)Game::Core::AnimationState::IDLE }, // Duplicate for safety
    { "DEATH",     (int)Game::Core::AnimationState::DEATH },

    // Bullet/Projectile States (Fixes your warnings!)
    // { "Player",    (int)Game::Core::AnimationState::PLAYER },    // For "player_missile"
    // { "Missile",   (int)Game::Core::AnimationState::MISSILE },   // Generic missile
    // { "InvaderA",  (int)Game::Core::AnimationState::INVADER_A }, // For "projectile_A"
    // { "InvaderB",  (int)Game::Core::AnimationState::INVADER_B }, // For "projectile_B"
    // { "InvaderC",  (int)Game::Core::AnimationState::INVADER_C }  // For "projectile_C"
});

// --- SOUND STATES ---
static bool s_sound_states = StateRegistry::Get().registerBatch("SoundState", {
    { "IDLE", (int)Game::Core::SoundState::IDLE },
    { "WALK", (int)Game::Core::SoundState::WALK },
    { "RUN" , (int)Game::Core::SoundState::RUN  },
    { "JUMP", (int)Game::Core::SoundState::JUMP },
    // You can add "SHOOT" here if needed later
});

// --- INVADER TYPES ---
static bool s_invader_states = StateRegistry::Get().registerBatch("InvaderType", {
    { "A", (int)Game::Core::InvaderType::INVADER_A },
    { "B", (int)Game::Core::InvaderType::INVADER_B },
    { "C", (int)Game::Core::InvaderType::INVADER_C }
});

// --- COLLISION LAYERS ---
using namespace Engine::Physics;
static bool s_collision_layers = StateRegistry::Get().registerBatch("CollisionLayer", {
    { "NONE",           (int)CollisionLayer::NONE },
    { "PLAYER",         (int)CollisionLayer::PLAYER },
    { "INVADER",        (int)CollisionLayer::INVADER },
    { "PLAYER_BULLET",  (int)CollisionLayer::PLAYER_BULLET },
    { "INVADER_BULLET", (int)CollisionLayer::INVADER_BULLET },
    { "WALL",           (int)CollisionLayer::WALL }
});
