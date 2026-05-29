#include "game/core/components/Tags.h"

#include "engine/core/GameObject.h"
#include "engine/scripting/ComponentRegistry.h"
#include "engine/scripting/StateRegistry.h"  // Needed for InvaderType

#include "game/core/GameTypes.h"  // For InvaderType enum definition

using namespace Engine::Scripting;

// // =================================================================================
// // 1. AI COMPONENT
// // =================================================================================
// static bool s_ai_registered = ComponentRegistry::Get().registerComponent(
//     "AI",
//     [](Engine::Core::GameObject& obj, const sol::table& t) {
//         auto& ai = obj.addComponent<Game::Control::AIComponent>();
//
//         ai.currentState    = t.get_or("currentState", 1); // 1 = Entry/Move
//         ai.stateRepetition = t.get_or("stateRepetition", 0);
//         ai.stateTimer      = t.get_or("stateTimer", 0.0f);
//         ai.stateLimit      = t.get_or("stateLimit", 1.0f);
//         ai.currentSpeed    = t.get_or("currentSpeed", 20.0f);
//     }
// );
//
// // =================================================================================
// // 2. INPUT (Tag)
// // =================================================================================
// static bool s_input_registered = ComponentRegistry::Get().registerComponent(
//     "Input",
//     [](Engine::Core::GameObject& obj, const sol::table& t) {
//         obj.addComponent<Game::Control::InputComponent>();
//     }
// );
//
// // =================================================================================
// // 3. INVADER (Updated for StateRegistry)
// // =================================================================================
// static bool s_invader_registered = ComponentRegistry::Get().registerComponent(
//     "Invader",
//     [](Engine::Core::GameObject& obj, const sol::table& t) {
//         auto& invader = obj.addComponent<Game::Control::InvaderComponent>();
//
//         // 1. Get Type as String ("A", "B", "C")
//         std::string typeStr = t.get_or("type", std::string("A"));
//
//         // 2. Convert to Enum using Registry
//         int typeVal = StateRegistry::Get().getState("InvaderType", typeStr);
//         invader.type = static_cast<int>(typeVal);
//
//         invader.score_value = t.get_or("score_value", 10);
//     }
// );
//
// // =================================================================================
// // 4. PLAYER
// // =================================================================================
// static bool s_player_registered = ComponentRegistry::Get().registerComponent(
//     "Player",
//     [](Engine::Core::GameObject& obj, const sol::table& t) {
//         auto& player = obj.addComponent<Game::Control::PlayerComponent>();
//
//         player.shoot_cooldown       = t.get_or("shoot_cooldown", 0.5f);
//         player.shoot_cooldown_timer = t.get_or("shoot_cooldown_timer", 0.0f);
//     }
// );
//
// // =================================================================================
// // 5. EMITTER
// // =================================================================================
// static bool s_emitter_registered = ComponentRegistry::Get().registerComponent(
//     "Emitter",
//     [](Engine::Core::GameObject& obj, const sol::table& t) {
//         auto& emitter = obj.addComponent<Game::Emitter::EmitterComponent>();
//
//         emitter.launch_speed   = t.get_or("launch_speed", 500.0f);
//         emitter.cooldown       = t.get_or("cooldown", 5.0f);
//         emitter.cooldown_timer = t.get_or("cooldown_timer", 0.0f);
//         emitter.active         = t.get_or("active", false);
//     }
// );
//
// // =================================================================================
// // 6. HEALTH
// // =================================================================================
// static bool s_health_registered = ComponentRegistry::Get().registerComponent(
//     "Health",
//     [](Engine::Core::GameObject& obj, const sol::table& t) {
//         auto& health = obj.addComponent<Game::Health::HealthComponent>();
//
//         health.max_health     = t.get_or("max_health", 100);
//         health.current_health = t.get_or("current_health", health.max_health);
//     }
// );
//
// // =================================================================================
// // 7. LIFETIME (Constructor Injection)
// // =================================================================================
// static bool s_lifetime_registered = ComponentRegistry::Get().registerComponent(
//     "Lifetime",
//     [](Engine::Core::GameObject& obj, const sol::table& t) {
//
//         float time = t.get_or("time_left", 5.0f);
//         // Explicitly passing argument to constructor
//         obj.addComponent<Game::Health::LifetimeComponent>(time);
//     }
// );
//
// // =================================================================================
// // 8. GAME STATE (Global Tracker)
// // =================================================================================
// static bool s_gamestate_registered = ComponentRegistry::Get().registerComponent(
//     "GameState",
//     [](Engine::Core::GameObject& obj, const sol::table& t) {
//
//         int score = t.get_or("score", 0);
//         int lives = t.get_or("lives", 3);
//         int num   = t.get_or("num_invaders", 0);
//
//         obj.addComponent<Game::UI::GameStateComponent>(score, lives, num);
//     }
// );
//
// // =================================================================================
// // 9. TAGS (Empty Components)
// // =================================================================================
//
// static bool s_score_registered = ComponentRegistry::Get().registerComponent("ScoreDisplayTag",
//     [](Engine::Core::GameObject& obj, const sol::table&) {
//         obj.addComponent<Game::Core::ScoreDisplayTag>();
// });
//
// static bool s_lives_registered = ComponentRegistry::Get().registerComponent("LivesDisplayTag",
//     [](Engine::Core::GameObject& obj, const sol::table&) {
//         obj.addComponent<Game::Core::LivesDisplayTag>();
// });
//
// static bool s_wall_registered  = ComponentRegistry::Get().registerComponent("WallComponent",
//     [](Engine::Core::GameObject& obj, const sol::table&) {
//         obj.addComponent<Game::Core::WallComponent>();
// });



static bool s_alive_registered  = ComponentRegistry::Get().registerComponent("IsAliveTag",
    [](Engine::Core::GameObject& obj, const sol::table&) {
        obj.addComponent<Game::Core::IsAliveTag>();
});


// =================================================================================
// 8. STATE REGISTRATION (Game Specifics)
// =================================================================================

// --- ANIMATION STATES ---
// Maps Lua Strings (from .json or .lua) to C++ Enums
static bool s_anim_states =
    StateRegistry::Get().registerBatch("AnimationState", {
                                                             // Basic States
                                                             {"IDLE", Game::Core::AnimationState::IDLE},  // Duplicate for safety
                                                             {"DEATH", Game::Core::AnimationState::DEATH},
                                                             {"RUN", Game::Core::AnimationState::RUN},

                                                             // Bullet/Projectile States (Fixes your warnings!)
                                                             // { "Player",    Game::Core::AnimationState::PLAYER },    // For "player_missile"
                                                             // { "Missile",   Game::Core::AnimationState::MISSILE },   // Generic missile
                                                             // { "InvaderA",  Game::Core::AnimationState::INVADER_A }, // For "projectile_A"
                                                             // { "InvaderB",  Game::Core::AnimationState::INVADER_B }, // For "projectile_B"
                                                             // { "InvaderC",  Game::Core::AnimationState::INVADER_C }  // For "projectile_C"
                                                         });

// --- SOUND STATES ---
static bool s_sound_states = StateRegistry::Get().registerBatch("SoundState", {
                                                                                  {"IDLE", Game::Core::SoundState::IDLE},
                                                                                  {"WALK", Game::Core::SoundState::WALK},
                                                                                  {"RUN", Game::Core::SoundState::RUN},
                                                                                  {"JUMP", Game::Core::SoundState::JUMP},
                                                                                  // You can add "SHOOT" here if needed later
                                                                              });

// --- INVADER TYPES ---
static bool s_invader_states = StateRegistry::Get().registerBatch(
    "InvaderType", {{"A", Game::Core::InvaderType::INVADER_A}, {"B", Game::Core::InvaderType::INVADER_B}, {"C", Game::Core::InvaderType::INVADER_C}});

// --- COLLISION LAYERS ---
using namespace Engine::Core;
static bool s_collision_layers = StateRegistry::Get().registerBatch("CollisionLayer", {{"NONE", (int)CollisionLayer::NONE},
                                                                                       {"PLAYER", (int)CollisionLayer::PLAYER},
                                                                                       {"INVADER", (int)CollisionLayer::INVADER},
                                                                                       {"PLAYER_BULLET", (int)CollisionLayer::PLAYER_BULLET},
                                                                                       {"INVADER_BULLET", (int)CollisionLayer::INVADER_BULLET},
                                                                                       {"WALL", (int)CollisionLayer::WALL}});


static bool bindPools() {


    SceneRegistry::Get().registerScene("Default", Game::Core::GamePools::DEFAULT);
    SceneRegistry::Get().registerScene("Static", Game::Core::GamePools::STATIC);
    SceneRegistry::Get().registerScene("Player", Game::Core::GamePools::PLAYER);
    SceneRegistry::Get().registerScene("Enemies", Game::Core::GamePools::ENEMIES);
    SceneRegistry::Get().registerScene("Bullets", Game::Core::GamePools::BULLETS);
    SceneRegistry::Get().registerScene("UI", Game::Core::GamePools::UI);

    return true;
}

using namespace Engine::Core;
static bool s_pools_layers = bindPools();