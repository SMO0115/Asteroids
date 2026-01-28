//
// Created by marcel on 9/28/25.
//

#include "glm/glm.hpp"


#include "Asteroids.h"

#include "engine/core/CoreModule.h"

#include "engine/modules/assets/AssetModule.h"
#include "engine/modules/audio/AudioModule.h"
#include "engine/modules/physics/PhysicsModule.h"
#include "engine/modules/graphics/RenderModule.h"

#include "game/modules/control/ControlModul.h"
#include "game/modules/emitter/EmiterModule.h"
#include "game/modules/health/HealthModule.h"
#include "game/modules/ui/UIModule.h"

#include "GameTypes.h"

#include "game/modules/control/InvaderComponent.h"
#include "game/modules/control/PlayerComponent.h"

#include "components/Tags.h"

#include "game/events/GameEvents.h"

#include "game/states/RunState.h"


namespace Game {
Asteroids::Asteroids() {

    m_player_control_system = std::make_unique<Control::PlayerControlSystem>();
    m_ai_system             = std::make_unique<Control::AISystem>();
    m_health_system         = std::make_unique<Health::HealthSystem>();
    m_lifetime_system       = std::make_unique<Health::LifetimeSystem>();
    m_score_system          = std::make_unique<UI::ScoreSystem>();
    m_ui_system             = std::make_unique<UI::UISystem>();
    m_emitter_system        = std::make_unique<Emitter::EmitterSystem>();
    m_event_bus             = std::make_unique<Events::GameEventBus>();

}

Asteroids::~Asteroids() {}


bool Asteroids::init(Engine::Application& engine) {

    Engine::Core::Scene& scene = engine.getScene();
    Pool& entities = scene.registerPool("Entities");
    Pool& ui = scene.registerPool("UI");
    Pool& GameState = scene.registerPool("GameState");

    engine.getScriptManager().runFile("assets/scripts/asset_loader.lua");
    engine.getScriptManager().runFile("assets/scripts/ent_creation.lua");


    m_game_session = std::make_unique<Engine::Core::GameObject>();
    m_game_session->addComponent<UI::GameStateComponent>(0, 3, 55);


    Engine::Assets::AssetManager&   asset_manager = engine.getAssetManager();
    Engine::Graphics::RenderSystem& renderer      = engine.getRenderer();

    Engine::Assets::resolveAssets(engine);

    // {
    //     std::unique_ptr<Engine::Core::GameObject> UI_Score = std::make_unique<Engine::Core::GameObject>();
    //
    //     Engine::Assets::Font* font = asset_manager.getFont("pixel", 24);
    //     UI_Score->addComponent<Engine::Graphics::UITextComponent>("Score: " + std::to_string(0), std::move(font), Engine::Core::Color{255, 255, 255}, 20, 10);
    //     UI_Score->addComponent<Core::ScoreDisplayTag>();
    //     m_ui_objects.push_back(std::move(UI_Score));
    // }
    // {
    //     std::unique_ptr<Engine::Core::GameObject> UI_Lives = std::make_unique<Engine::Core::GameObject>();
    //
    //     Engine::Assets::Font* font = asset_manager.getFont("pixel", 24);
    //     UI_Lives->addComponent<Engine::Graphics::UITextComponent>("Lives: " + std::to_string(0), std::move(font), Engine::Core::Color{255, 255, 255}, 690, 10);
    //     UI_Lives->addComponent<Core::LivesDisplayTag>();
    //     m_ui_objects.push_back(std::move(UI_Lives));
    // }





    //
    //
    //
    //
    //
    //
    //
    // {
    //     std::unique_ptr<Engine::Core::GameObject> player = std::make_unique<Engine::Core::GameObject>();
    //
    //     Engine::Graphics::AnimationComponent animation_component;
    //     animation_component.animations[static_cast<int>(Core::AnimationState::IDLE)]  = asset_manager.getAnimation("player");
    //     animation_component.animations[static_cast<int>(Core::AnimationState::DEATH)] = asset_manager.getAnimation("player_death");
    //     animation_component.current_state                                             = static_cast<int>(Core::AnimationState::IDLE);
    //
    //
    //     Engine::Physics::ColliderComponent collider_component;
    //     collider_component.bounds.w = animation_component.animations[static_cast<int>(Core::AnimationState::IDLE)]->frames[0].w;
    //     collider_component.bounds.h = animation_component.animations[static_cast<int>(Core::AnimationState::IDLE)]->frames[0].h;
    //     collider_component.layer    = Engine::Physics::CollisionLayer::PLAYER;
    //     collider_component.mask     = Engine::Physics::CollisionLayer::INVADER | Engine::Physics::CollisionLayer::INVADER_BULLET;
    //     player->addComponent<Engine::Physics::ColliderComponent>(std::move(collider_component));
    //     player->addComponent<Engine::Graphics::AnimationComponent>(std::move(animation_component));
    //
    //     Engine::Audio::SoundComponent sound_component;
    //     sound_component.sounds[static_cast<int>(Core::SoundState::IDLE)] = "fastinvader1";  // asset_manager.getSound("fastinvader1");
    //     sound_component.current_state                                    = static_cast<int>(Core::SoundState::IDLE);
    //     player->addComponent<Engine::Audio::SoundComponent>(std::move(sound_component));
    //
    //
    //     player->addComponent<Engine::Core::TransformComponent>();
    //     player->getComponent<Engine::Core::TransformComponent>().position = glm::vec2(400, 750);
    //     player->getComponent<Engine::Core::TransformComponent>().scale    = 2;
    //     player->addComponent<Engine::Physics::PhysicsComponent>();
    //     player->addComponent<Engine::Graphics::SpriteComponent>();
    //     player->addComponent<Control::InputComponent>();
    //     player->addComponent<Control::PlayerComponent>();
    //
    //     player->addComponent<Health::HealthComponent>();
    //     player->getComponent<Health::HealthComponent>().max_health     = 100;
    //     player->getComponent<Health::HealthComponent>().current_health = 100;
    //
    //     entities.push_back(std::move(player));
    // }
    //
    //
    // for (int k = 0; k < 4; k++) {
    //
    //     glm::vec2 wall_offset(100 + k * 180, 630);
    //
    //     for (int i = 0; i < 4; i++) {
    //         for (int j = 0; j < 4; j++) {
    //
    //             {
    //                 std::unique_ptr<Engine::Core::GameObject> wall    = std::make_unique<Engine::Core::GameObject>();
    //                 std::string                               wall_id = "wall_" + std::to_string(k) + "_" + std::to_string(i) + "_" + std::to_string(j);
    //
    //                 Engine::Graphics::AnimationComponent animation_component;
    //                 animation_component.animations[static_cast<int>(Core::AnimationState::IDLE)]            = asset_manager.getAnimation(wall_id);
    //                 animation_component.animations[static_cast<int>(Core::AnimationState::IDLE)]->frames[0] = {11 * i, 8 * j, 11, 8};
    //                 animation_component.animations[static_cast<int>(Core::AnimationState::DEATH)]           = nullptr;
    //                 animation_component.current_state                                                       = static_cast<int>(Core::AnimationState::IDLE);
    //
    //                 // wall->addComponent<Core::WallComponent>();
    //
    //                 Engine::Physics::ColliderComponent collider_component;
    //                 collider_component.bounds.w = animation_component.animations[static_cast<int>(Core::AnimationState::IDLE)]->frames[0].w;
    //                 collider_component.bounds.h = animation_component.animations[static_cast<int>(Core::AnimationState::IDLE)]->frames[0].h;
    //                 collider_component.layer    = Engine::Physics::CollisionLayer::WALL;
    //                 collider_component.mask     = Engine::Physics::CollisionLayer::INVADER_BULLET | Engine::Physics::CollisionLayer::PLAYER_BULLET;
    //                 wall->addComponent<Engine::Physics::ColliderComponent>(std::move(collider_component));
    //                 wall->addComponent<Engine::Graphics::AnimationComponent>(std::move(animation_component));
    //
    //                 // Engine::Audio::SoundComponent sound_component;
    //                 // sound_component.sounds[ static_cast<int>( Core::SoundState::IDLE ) ] = "fastinvader1"; //asset_manager.getSound("fastinvader1");
    //                 // sound_component.current_state = static_cast<int>( Core::SoundState::IDLE );
    //                 // wall->addComponent<Engine::Audio::SoundComponent>(std::move(sound_component));
    //
    //
    //                 wall->addComponent<Engine::Core::TransformComponent>();
    //                 wall->getComponent<Engine::Core::TransformComponent>().position = wall_offset + glm::vec2(i * 11 * 2, j * 8 * 2);
    //                 wall->getComponent<Engine::Core::TransformComponent>().scale    = 2;
    //                 wall->addComponent<Engine::Graphics::SpriteComponent>();
    //
    //                 wall->addComponent<Health::HealthComponent>();
    //                 wall->getComponent<Health::HealthComponent>().max_health     = 400;
    //                 wall->getComponent<Health::HealthComponent>().current_health = 400;
    //
    //
    //                 entities.push_back(std::move(wall));
    //             }
    //
    //
    //             {
    //                 std::unique_ptr<Engine::Core::GameObject> wall = std::make_unique<Engine::Core::GameObject>();
    //                 std::string wall_destruction_id = "wall_destruction_" + std::to_string(k) + "_" + std::to_string(i) + "_" + std::to_string(j);
    //
    //                 Engine::Graphics::AnimationComponent animation_component;
    //                 animation_component.animations[static_cast<int>(Core::AnimationState::IDLE)]            = asset_manager.getAnimation(wall_destruction_id);
    //                 animation_component.animations[static_cast<int>(Core::AnimationState::IDLE)]->frames[0] = {0, 0, 11, 8};
    //                 animation_component.animations[static_cast<int>(Core::AnimationState::DEATH)]           = nullptr;
    //                 animation_component.current_state                                                       = static_cast<int>(Core::AnimationState::IDLE);
    //
    //                 wall->addComponent<Core::WallComponent>();
    //
    //                 Engine::Physics::ColliderComponent collider_component;
    //                 collider_component.bounds.w = animation_component.animations[static_cast<int>(Core::AnimationState::IDLE)]->frames[0].w;
    //                 collider_component.bounds.h = animation_component.animations[static_cast<int>(Core::AnimationState::IDLE)]->frames[0].h;
    //                 collider_component.layer    = Engine::Physics::CollisionLayer::WALL;
    //                 collider_component.mask     = Engine::Physics::CollisionLayer::INVADER_BULLET | Engine::Physics::CollisionLayer::PLAYER_BULLET;
    //                 wall->addComponent<Engine::Physics::ColliderComponent>(std::move(collider_component));
    //                 wall->addComponent<Engine::Graphics::AnimationComponent>(std::move(animation_component));
    //
    //                 // Engine::Audio::SoundComponent sound_component;
    //                 // sound_component.sounds[ static_cast<int>( Core::SoundState::IDLE ) ] = "fastinvader1"; //asset_manager.getSound("fastinvader1");
    //                 // sound_component.current_state = static_cast<int>( Core::SoundState::IDLE );
    //                 // wall->addComponent<Engine::Audio::SoundComponent>(std::move(sound_component));
    //
    //
    //                 wall->addComponent<Engine::Core::TransformComponent>();
    //                 wall->getComponent<Engine::Core::TransformComponent>().position = wall_offset + glm::vec2(i * 11 * 2, j * 8 * 2);
    //                 wall->getComponent<Engine::Core::TransformComponent>().scale    = 2;
    //                 wall->addComponent<Engine::Graphics::SpriteComponent>();
    //
    //                 wall->addComponent<Health::HealthComponent>();
    //                 wall->getComponent<Health::HealthComponent>().max_health     = 400;
    //                 wall->getComponent<Health::HealthComponent>().current_health = 400;
    //
    //                 entities.push_back(std::move(wall));
    //             }
    //         }
    //     }
    // }
    //
    //
    // std::size_t                      N             = 11;
    // std::size_t                      M             = 5;
    // std::size_t                      spacing       = 60;
    // std::array<std::string, 5>       s_enemy_types = {"invader_A", "invader_B", "invader_B", "invader_C", "invader_C"};
    // std::array<Core::InvaderType, 5> e_enemy_types = {Core::InvaderType::INVADER_A, Core::InvaderType::INVADER_B, Core::InvaderType::INVADER_B,
    //                                                   Core::InvaderType::INVADER_C, Core::InvaderType::INVADER_C};
    // for (int i = 0; i < N; i++) {
    //     for (int j = 0; j < M; j++) {
    //
    //         std::unique_ptr<Engine::Core::GameObject> enemy = std::make_unique<Engine::Core::GameObject>();
    //
    //
    //         Engine::Graphics::AnimationComponent animation_component;
    //         animation_component.animations[static_cast<int>(Core::AnimationState::IDLE)]  = asset_manager.getAnimation(s_enemy_types[j]);
    //         animation_component.animations[static_cast<int>(Core::AnimationState::DEATH)] = asset_manager.getAnimation("invader_death");
    //         animation_component.current_state                                             = static_cast<int>(Core::AnimationState::IDLE);
    //         animation_component.current_frame                                             = j % 2;
    //
    //         Engine::Physics::ColliderComponent collider_component;
    //         collider_component.bounds.w = animation_component.animations[static_cast<int>(Core::AnimationState::IDLE)]->frames[0].w;
    //         collider_component.bounds.h = animation_component.animations[static_cast<int>(Core::AnimationState::IDLE)]->frames[0].h;
    //         collider_component.layer    = Engine::Physics::CollisionLayer::INVADER;
    //         collider_component.mask     = Engine::Physics::CollisionLayer::PLAYER_BULLET | Engine::Physics::CollisionLayer::PLAYER;
    //         enemy->addComponent<Engine::Physics::ColliderComponent>(std::move(collider_component));
    //         enemy->addComponent<Engine::Graphics::AnimationComponent>(std::move(animation_component));
    //
    //
    //         Engine::Audio::SoundComponent sound_component;
    //         sound_component.sounds[static_cast<int>(Core::SoundState::IDLE)] = "fastinvader1";  // asset_manager.getSound("fastinvader1");
    //         sound_component.current_state                                    = static_cast<int>(Core::SoundState::IDLE);
    //         enemy->addComponent<Engine::Audio::SoundComponent>(std::move(sound_component));
    //
    //         enemy->addComponent<Engine::Core::TransformComponent>();
    //         enemy->getComponent<Engine::Core::TransformComponent>().position = glm::vec2(35 + i * spacing, 100 + j * spacing);
    //         enemy->getComponent<Engine::Core::TransformComponent>().scale    = 1.5;
    //
    //         enemy->addComponent<Control::InvaderComponent>();
    //         enemy->getComponent<Control::InvaderComponent>().type        = e_enemy_types[j];
    //         enemy->getComponent<Control::InvaderComponent>().score_value = static_cast<int>(e_enemy_types[j]) * (j + 1);
    //
    //         enemy->addComponent<Engine::Physics::PhysicsComponent>();
    //         enemy->addComponent<Engine::Graphics::SpriteComponent>();
    //         enemy->addComponent<Control::AIComponent>();
    //
    //         enemy->addComponent<Health::HealthComponent>();
    //         enemy->getComponent<Health::HealthComponent>().max_health     = 100;
    //         enemy->getComponent<Health::HealthComponent>().current_health = 100;
    //
    //         entities.push_back(std::move(enemy));
    //     }
    // }
    //
    //
    // for (int i = 0; i < N * M + 1; i++) {
    //
    //     std::unique_ptr<Engine::Core::GameObject> bullet = std::make_unique<Engine::Core::GameObject>();
    //
    //     Engine::Graphics::AnimationComponent animation_component;
    //     animation_component.animations[static_cast<int>(Core::EmittorState::PLAYER)]    = asset_manager.getAnimation("player_missile");
    //     animation_component.animations[static_cast<int>(Core::EmittorState::MISSILE)]   = asset_manager.getAnimation("missile");
    //     animation_component.animations[static_cast<int>(Core::EmittorState::INVADER_A)] = asset_manager.getAnimation("projectile_A");
    //     animation_component.animations[static_cast<int>(Core::EmittorState::INVADER_B)] = asset_manager.getAnimation("projectile_B");
    //     animation_component.animations[static_cast<int>(Core::EmittorState::INVADER_C)] = asset_manager.getAnimation("projectile_C");
    //
    //     animation_component.current_state = static_cast<int>(Core::EmittorState::PLAYER);
    //     animation_component.speed         = 30.0f;
    //
    //
    //     Engine::Physics::ColliderComponent collider_component;
    //     collider_component.bounds.w = animation_component.animations[static_cast<int>(Core::EmittorState::PLAYER)]->frames[0].w;
    //     collider_component.bounds.h = animation_component.animations[static_cast<int>(Core::EmittorState::PLAYER)]->frames[0].h;
    //     collider_component.layer    = Engine::Physics::CollisionLayer::INVADER_BULLET;
    //     collider_component.mask     = Engine::Physics::CollisionLayer::PLAYER;
    //     bullet->addComponent<Engine::Physics::ColliderComponent>();
    //     bullet->addComponent<Engine::Graphics::AnimationComponent>(std::move(animation_component));
    //
    //
    //     Engine::Audio::SoundComponent sound_component;
    //     sound_component.sounds[static_cast<int>(Core::SoundState::IDLE)] = "shoot";  // asset_manager.getSound("fastinvader1");
    //     sound_component.current_state                                    = static_cast<int>(Core::SoundState::IDLE);
    //     bullet->addComponent<Engine::Audio::SoundComponent>(std::move(sound_component));
    //
    //     bullet->addComponent<Engine::Core::TransformComponent>();
    //     bullet->getComponent<Engine::Core::TransformComponent>().position = glm::vec2(400, 400);
    //     bullet->getComponent<Engine::Core::TransformComponent>().scale    = 2.0f;
    //     bullet->addComponent<Engine::Graphics::SpriteComponent>();
    //     bullet->addComponent<Engine::Physics::PhysicsComponent>();
    //     bullet->addComponent<Emitter::EmitterComponent>();
    //
    //     bullet->addComponent<Health::HealthComponent>();
    //
    //     bullet->setActive(false);
    //
    //     entities.push_back(std::move(bullet));
    // }

    m_state = std::make_unique<State::RunState>(*this);
    m_state->onEnter();

    return true;
}


void Asteroids::handleInput(Engine::Input::InputManager& input_manager) {

    std::unique_ptr<Engine::Core::IGameState> new_state = m_state->handleInput(input_manager);

    if (new_state != nullptr) {

        m_state->onExit();
        m_state = std::move(new_state);
        m_state->onEnter();
    }
}


bool Asteroids::update(float deltatime, Engine::Application& engine) {

    std::unique_ptr<Engine::Core::IGameState> new_state = m_state->update(deltatime, engine);

    if (new_state != nullptr) {

        m_state->onExit();
        m_state = std::move(new_state);
        m_state->onEnter();
    }

    m_event_bus->clear();

    return true;
}

void Asteroids::render(Engine::Application& engine) { m_state->render(engine); }
}  // namespace Game
