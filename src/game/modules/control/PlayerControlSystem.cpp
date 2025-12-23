//
// Created by marcel on 11/8/25.
//

#include "PlayerControlSystem.h"

#include "engine/core/CoreModule.h"
#include "game/core/CoreModule.h"

#include "engine/modules/input/InputModule.h"
#include "engine/modules/physics/PhysicsModule.h"
#include "engine/modules/rendering/RenderModule.h"

namespace Game::Control {
PlayerControlSystem::PlayerControlSystem()  = default;
PlayerControlSystem::~PlayerControlSystem() = default;

void PlayerControlSystem::update(float deltaTime, Engine::Input::InputManager& input_manager, Events::GameEventBus& game_event_bus,
                                 Engine::Events::EngineEventBus& engine_event_bus, const std::vector<std::unique_ptr<Engine::Core::GameObject>>& game_objects) {
    for (auto& object : game_objects) {
        if (!object->hasComponent<Control::PlayerComponent>()) continue;
        if (!object->isActive()) continue;

        Engine::Physics::PhysicsComponent& physics_component = object->getComponent<Engine::Physics::PhysicsComponent>();
        Engine::Core::TransformComponent&  transform         = object->getComponent<Engine::Core::TransformComponent>();
        Engine::Graphics::SpriteComponent& sprite_component  = object->getComponent<Engine::Graphics::SpriteComponent>();
        Control::PlayerComponent&          player_component  = object->getComponent<Control::PlayerComponent>();

        player_component.shoot_cooldown_timer -= deltaTime;
        physics_component.velocity.x = 0;

        const int frame_left  = transform.position.x - sprite_component.sourceRect.w - 5;
        const int frame_right = transform.position.x + sprite_component.sourceRect.w + 5;

        const float moveSpeed = 5.0f;
        if (input_manager.isKeyHeld(Engine::Input::Key::LEFT)) {
            if (frame_left > 0) {
                physics_component.velocity.x = -moveSpeed;
            }
        }
        if (input_manager.isKeyHeld(Engine::Input::Key::RIGHT)) {
            if (frame_right < 800) {
                physics_component.velocity.x = moveSpeed;
            }
        }

        if (input_manager.isKeyHeld(Engine::Input::Key::SPACE)) {
            if (player_component.shoot_cooldown_timer <= 0) {
                player_component.shoot_cooldown_timer = player_component.shoot_cooldown;

                auto position = transform.position;
                position.y += -30;

                game_event_bus.publish<Events::EmitterEvent>(static_cast<int>(Core::EmittorState::PLAYER), Engine::Physics::CollisionLayer::PLAYER_BULLET,
                                                             Engine::Physics::CollisionLayer::INVADER | Engine::Physics::CollisionLayer::WALL, position,
                                                             glm::vec2{0.0f, -10.0f});

                engine_event_bus.publish<Engine::Events::PlaySoundEvent>("shoot");
            }
        }
    }
}
}  // namespace Game::Control
