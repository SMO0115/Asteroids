//
// Created by marcel on 1/24/26.
//

#include "ComponentBinding.h"

#include <unordered_map>

#include "engine/core/Log.h"

#include "engine/core/components/LifeComponent.h"
#include "engine/core/components/TransformComponent.h"
#include "engine/modules/audio/SoundComponent.h"
#include "engine/modules/graphics/AnimationComponent.h"
#include "engine/modules/graphics/SpriteComponent.h"
#include "engine/modules/graphics/UITextComponent.h"
#include "engine/modules/physics/ColliderComponent.h"
#include "engine/modules/physics/PhysicsComponent.h"

#include "engine/core/GameObject.h"
#include "engine/scripting/ComponentRegistry.h"
#include "engine/scripting/StateRegistry.h"

namespace Engine::Scripting {

// A plain function, called explicitly by the Application. It replaces the 'static bool s_x =
// Registry::Get().registerComponent(...)' trick: a static library drops object files that nothing
// references, so those registrations silently never ran unless this .cpp was #included in a header.
void registerEngineComponents(ComponentRegistry& components, const StateRegistry& states) {

    // The factories run later (whenever Lua spawns an entity), so they keep a pointer to the
    // registry they need, instead of asking a global singleton
    const StateRegistry* state_names = &states;

    // =================================================================================
    // 1. LIFE
    // =================================================================================
    components.registerComponent("Life", [](Engine::Core::GameObject& obj, const sol::table& t) {
        auto& life = obj.addComponent<Engine::Core::LifeComponent>();

        // The default's type must match the member: health is an int, life_time a float
        life.current_health    = t.get_or("currentHealth", 0);
        life.max_health        = t.get_or("maxHealth", 0);
        life.life_time         = t.get_or("lifeTime", 0.f);
        life.destroy_on_expire = t.get_or("destroyOnExpire", false);
    });

    // =================================================================================
    // 1. TRANSFORM
    // =================================================================================
    components.registerComponent("Transform", [](Engine::Core::GameObject& obj, const sol::table& t) {
        auto& transform = obj.addComponent<Engine::Core::TransformComponent>();
        // 0.f, not 0: the type of the default decides what sol2 returns, and an int would cut off x = 12.5
        transform.position = glm::vec2(t.get_or("x", 0.f), t.get_or("y", 0.f));
        transform.scale    = t.get_or("scale", 1.f);
        transform.rotation = t.get_or("rotation", 0.f);
    });

    // =================================================================================
    // 2. SOUND (UPDATED)
    // =================================================================================
    components.registerComponent("Sound", [state_names](Engine::Core::GameObject& obj, const sol::table& t) {
        auto& sound      = obj.addComponent<Engine::Audio::SoundComponent>();
        sound.is_looping = t.get_or("is_looping", false);

        // A. Set Current State (String -> Int)
        // Lua: current_state = "IDLE"
        std::string stateStr = t.get_or("current_state", std::string("IDLE"));
        sound.current_state  = state_names->getState("SoundState", stateStr);

        // B. Load Sound Map (String Key -> Int Key)
        // Lua: sounds = { IDLE = "assets/sounds/idle.wav", WALK = "..." }
        if (sol::optional<sol::table> sounds_table = t["sounds"]) {
            for (const auto& kv : *sounds_table) {
                // Key is String ("IDLE"), Value is String ("path")
                if (kv.first.is<std::string>() && kv.second.is<std::string>()) {

                    std::string keyState  = kv.first.as<std::string>();
                    std::string assetPath = kv.second.as<std::string>();

                    // Resolve "IDLE" -> 0
                    int stateID = state_names->getState("SoundState", keyState);

                    // Store: [0] = "assets/sounds/idle.wav"
                    sound.sounds[stateID] = assetPath;
                }
            }
        }
    });

    // =================================================================================
    // 3. COLLIDER
    // =================================================================================
    components.registerComponent("Collider", [state_names](Engine::Core::GameObject& obj, const sol::table& t) {
        auto& collider = obj.addComponent<Engine::Physics::ColliderComponent>();

        // 1. Shape: a circle. Lua: radius = 20, offset = { x = 7, y = 0 } (offset is optional)
        collider.radius = t.get_or("radius", 0.f);
        if (sol::optional<sol::table> offset = t["offset"]) {
            collider.offset = glm::vec2(offset->get_or("x", 0.f), offset->get_or("y", 0.f));
        }
        if (collider.radius <= 0.f) {
            // e.g. a typo, or an old 'bounds = {...}' table: it would silently never collide
            ENGINE_WARN("[Script] Collider without a radius never collides");
        }

        // 2. Helper: turn layer names into bits. The engine doesn't know any layer names: the game
        // registered what "PLAYER", "ASTEROID", ... mean under "CollisionLayer".
        auto parseMask = [state_names](const sol::object& input) -> Engine::Physics::CollisionMask {
            Engine::Physics::CollisionMask mask = 0;

            // Helper to lookup ONE string in the registry
            auto resolve = [state_names](const std::string& key) {
                return static_cast<Engine::Physics::CollisionMask>(state_names->getState("CollisionLayer", key));
            };

            // Case A: Single String ("PLAYER")
            if (input.is<std::string>()) {
                mask = resolve(input.as<std::string>());
            }
            // Case B: List of Strings ({"PLAYER", "ASTEROID"})
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

        // 3. Assign. get<sol::object> converts explicitly. Passing the t["layer"] proxy made the
        // compiler choose between two possible conversions to sol::object (-Wconversion).
        if (sol::object layer = t.get<sol::object>("layer"); layer.valid()) collider.layer = parseMask(layer);

        if (sol::object mask = t.get<sol::object>("mask"); mask.valid()) collider.mask = parseMask(mask);
    });


    // =================================================================================
    // 4. PHYSICS
    // =================================================================================
    components.registerComponent("Physics", [](Engine::Core::GameObject& obj, const sol::table& t) {
        auto& phys = obj.addComponent<Engine::Physics::PhysicsComponent>();

        auto getVec2 = [&](const std::string& key) -> glm::vec2 {
            sol::optional<sol::table> vec = t[key];
            if (vec) return glm::vec2(vec->get_or("x", 0.f), vec->get_or("y", 0.f));
            return glm::vec2(0.f);
        };

        phys.velocity     = getVec2("velocity");
        phys.acceleration = getVec2("acceleration");
        phys.damping      = t.get_or("damping", 1.0f);


        phys.angularVelocity     = t.get_or("angularVelocity", 0.0f);
        phys.angularDamping      = t.get_or("angularDamping", 1.0f);
        phys.angularAcceleration = t.get_or("angularAcceleration", 0.0f);

        phys.mass = t.get_or("mass", 1.0f);
    });

    // =================================================================================
    // 5. ANIMATION (UPDATED)
    // =================================================================================
    components.registerComponent("Animation", [state_names](Engine::Core::GameObject& obj, const sol::table& t) {
        auto& anim = obj.addComponent<Engine::Graphics::AnimationComponent>();
        anim.speed = t.get_or("speed", 1.0f);

        // A. Set Initial State (String -> Int)
        std::string curState = t.get_or("current_state", std::string("IDLE"));
        anim.current_state   = state_names->getState("AnimationState", curState);

        // B. Load Animations Map (String Key -> Int Key)
        // Lua: animations = { IDLE = "anim_player_idle.json", RUN = "..." }
        sol::optional<sol::table> anim_table = t["animations"];
        if (anim_table) {
            for (const auto& kv : *anim_table) {
                if (kv.first.is<std::string>() && kv.second.is<std::string>()) {

                    std::string stateName = kv.first.as<std::string>();
                    std::string assetId   = kv.second.as<std::string>();

                    // Resolve "IDLE" -> 0
                    int stateID = state_names->getState("AnimationState", stateName);

                    anim.animations[stateID] = assetId;
                }
            }
        }
    });

    // =================================================================================
    // 6. SPRITE
    // =================================================================================
    components.registerComponent("Sprite", [](Engine::Core::GameObject& obj, const sol::table& t) {
        auto& sprite = obj.addComponent<Engine::Graphics::SpriteComponent>();

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
    });

    // =================================================================================
    // 7. UI TEXT
    // =================================================================================
    components.registerComponent("UIText", [](Engine::Core::GameObject& obj, const sol::table& t) {
        auto& ui = obj.addComponent<Engine::Graphics::UITextComponent>();

        ui.font_name = t.get_or("font_name", std::string("default_font"));
        ui.text      = t.get_or("text", std::string("New Text"));
        ui.size      = t.get_or("size", 12);

        ui.x_offset = t.get_or("x_offset", 0);
        ui.y_offset = t.get_or("y_offset", 0);

        if (sol::optional<sol::table> col = t["color"]) {
            ui.color.r = col->get_or("r", 255);
            ui.color.g = col->get_or("g", 255);
            ui.color.b = col->get_or("b", 255);
            ui.color.a = col->get_or("a", 255);
        }
    });
}
}  // namespace Engine::Scripting
