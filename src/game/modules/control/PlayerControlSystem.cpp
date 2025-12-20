//
// Created by marcel on 11/8/25.
//


#include "PlayerControlSystem.h"

#include "engine/core/CoreModule.h"
#include "game/core/CoreModule.h"

#include "engine/modules/physics/PhysicsModule.h"
#include "engine/modules/rendering/RenderModule.h"
#include "engine/modules/input/InputModule.h"




PlayerControlSystem::PlayerControlSystem() = default;
PlayerControlSystem::~PlayerControlSystem() = default;


void PlayerControlSystem::update(float deltaTime, InputManager& input_manager, GameEventBus& game_event_bus, EngineEventBus& engine_event_bus, const std::vector<std::unique_ptr<GameObject>>& game_objects) {

    for (auto& object : game_objects) {

        if (!object->hasComponent<PlayerComponent>()) continue;
        if (!object->isActive()) continue;

        PhysicsComponent& physics_component = object->getComponent<PhysicsComponent>();
        TransformComponent& transform       = object->getComponent<TransformComponent>();
        SpriteComponent& sprite_component   = object->getComponent<SpriteComponent>();
        PlayerComponent& player_component   = object->getComponent<PlayerComponent>();


        player_component.shoot_cooldown_timer -= deltaTime;
        physics_component.velocity.x = 0;

        const int frame_left  = transform.position.x - sprite_component.sourceRect.w - 5;
        const int frame_right = transform.position.x + sprite_component.sourceRect.w + 5;


        const float moveSpeed = 5.0f;
        if (input_manager.isKeyHeld(Key::LEFT)) {
            if (frame_left > 0) {
                physics_component.velocity.x = -moveSpeed;
            }
        }
        if (input_manager.isKeyHeld(Key::RIGHT)) {
            if (frame_right < 800) {
                physics_component.velocity.x = moveSpeed;
            }
        }

        if (input_manager.isKeyHeld(Key::SPACE)) {

            if (player_component.shoot_cooldown_timer <= 0) {

                player_component.shoot_cooldown_timer = player_component.shoot_cooldown;

                auto position = transform.position;
                position.y += -30;

                game_event_bus.publish<EmitterEvent>(
                    static_cast<int>(EmittorState::PLAYER),
                    CollisionLayer::PLAYER_BULLET,
                    CollisionLayer::INVADER | CollisionLayer::WALL,
                    position,
                    glm::vec2{0.0f, -10.0f}
                );

                engine_event_bus.publish<PlaySoundEvent>("shoot");
            }
        }
    }
}


