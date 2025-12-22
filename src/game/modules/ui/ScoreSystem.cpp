//
// Created by marcel on 12/16/25.
//

#include "ScoreSystem.h"

#include "engine/core/CoreModule.h"
#include "engine/modules/rendering/RenderModule.h"
#include "engine/modules/physics/PhysicsModule.h"
#include "engine/modules/assets/AssetModule.h"

#include "game/core/CoreModule.h"
#include "game/modules/health/HealthModule.h"
#include "game/modules/control/ControlModul.h"
#include "GameStateComponent.h"


namespace Game::UI {
    void ScoreSystem::update(float deltaTime, Events::GameEventBus& game_event_bus, GameStateComponent &game_state) {

        for (const auto& event : game_event_bus.getEvents<Events::DeathEvent>()) {

            if (event.object->hasComponent<Control::PlayerComponent>()) {

                game_state.lives -= 1;

                if (game_state.lives > 0) {

                    event.object->getComponent<Engine::Core::TransformComponent>().position = glm::vec2(400, 750);;
                    event.object->getComponent<Engine::Graphics::AnimationComponent>().current_state = static_cast<int>( Core::AnimationState::IDLE );
                    event.object->setActive(true);


                    event.object->removeComponent<Health::LifetimeComponent>();

                    Engine::Graphics::AnimationComponent& animation_component = event.object->getComponent<Engine::Graphics::AnimationComponent>();

                    Engine::Physics::ColliderComponent collider_component;
                    collider_component.bounds.w = animation_component.animations[ static_cast<int>( Core::AnimationState::IDLE ) ]->frames[0].w;
                    collider_component.bounds.h = animation_component.animations[ static_cast<int>( Core::AnimationState::IDLE ) ]->frames[0].h;
                    collider_component.layer = Engine::Physics::CollisionLayer::PLAYER;
                    collider_component.mask = Engine::Physics::CollisionLayer::INVADER | Engine::Physics::CollisionLayer::INVADER_BULLET;

                    event.object->addComponent<Engine::Physics::ColliderComponent>(std::move(collider_component));

                    event.object->getComponent<Health::HealthComponent>().current_health = event.object->getComponent<Health::HealthComponent>().max_health;
                }
            }

            if (event.object->hasComponent<Control::AIComponent>()) {

                game_state.score += 100;
                game_state.num_invaders--;
            }

        }
    }
}