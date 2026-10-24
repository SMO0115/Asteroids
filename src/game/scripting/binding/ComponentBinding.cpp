#include "ComponentBinding.h"

#include "engine/core/GameObject.h"
#include "engine/core/Scene.h"
#include "engine/scripting/ComponentRegistry.h"
#include "engine/scripting/StateRegistry.h"

#include "game/core/GameTypes.h"
#include "game/core/components/Tags.h"
#include "game/modules/asteroid/AsteroidComponent.h"
#include "game/modules/player/ShipComponent.h"

namespace Game::Scripting {

// =================================================================================
// 1. COMPONENTS (the game's own; the engine registers Transform, Physics, ... itself)
// =================================================================================
void registerGameComponents(Engine::Scripting::ComponentRegistry& components) {

    // Tags (Empty Components)
    components.registerComponent("BulletTag", [](Engine::Core::GameObject& obj, const sol::table&) { obj.addComponent<Core::BulletTag>(); });

    // Lua: Asteroid = { size = 3 }
    components.registerComponent("Asteroid", [](Engine::Core::GameObject& obj, const sol::table& t) {
        auto& asteroid = obj.addComponent<Asteroid::AsteroidComponent>();
        asteroid.size  = t.get_or("size", 3);
    });

    // Lua: Ship = {}   (it always starts FLYING)
    components.registerComponent("Ship", [](Engine::Core::GameObject& obj, const sol::table&) { obj.addComponent<Player::ShipComponent>(); });
}

// =================================================================================
// 2. STATE NAMES (Maps Lua Strings (from .json or .lua) to C++ Enums)
// =================================================================================
void registerGameStates(Engine::Scripting::StateRegistry& states) {

    states.registerBatch("AnimationState", {
                                               {"IDLE", Core::AnimationState::IDLE},
                                               {"DEATH", Core::AnimationState::DEATH},
                                               {"RUN", Core::AnimationState::RUN},
                                           });

    states.registerBatch("SoundState", {
                                           {"IDLE", Core::SoundState::IDLE},
                                           {"RUN", Core::SoundState::RUN},
                                       });

    // The names of OUR collision bits: the engine's Collider only sees numbers
    states.registerBatch("CollisionLayer", {
                                               {"NONE", Core::CollisionLayer::NONE},
                                               {"PLAYER", Core::CollisionLayer::PLAYER},
                                               {"ASTEROID", Core::CollisionLayer::ASTEROID},
                                               {"PLAYER_BULLET", Core::CollisionLayer::PLAYER_BULLET},
                                           });
}

// =================================================================================
// 3. POOL NAMES (so Lua can say SpawnEntity(..., "Enemies", ...))
// =================================================================================
void registerGamePools(Engine::Core::Scene& scene) {

    scene.registerName(Core::GamePools::DEFAULT, "Default");
    scene.registerName(Core::GamePools::STATIC, "Static");
    scene.registerName(Core::GamePools::PLAYER, "Player");
    scene.registerName(Core::GamePools::ENEMIES, "Enemies");
    scene.registerName(Core::GamePools::BULLETS, "Bullets");
    scene.registerName(Core::GamePools::EFFECTS, "Effects");
    scene.registerName(Core::GamePools::UI, "UI");
}
}  // namespace Game::Scripting
