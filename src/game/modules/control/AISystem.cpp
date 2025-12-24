//
// Created by marcel on 11/8/25.
//

#include "AISystem.h"

#include "engine/core/CoreModule.h"
#include "game/core/CoreModule.h"

#include "AIComponent.h"
#include "engine/utils/Random.h"


#include "engine/modules/audio/AudioModule.h"
#include "engine/modules/physics/PhysicsModule.h"
#include "engine/modules/rendering/RenderModule.h"

namespace Game::Control {
void AISystem::update(float deltaTime, Engine::Events::EngineEventBus& engine_event_bus, Events::GameEventBus& game_event_bus,
                      std::vector<std::unique_ptr<Engine::Core::GameObject> >& game_objects) {

    for (const auto& object : game_objects) {


        if (!object->hasComponent<AIComponent>()) continue;
        if (!object->hasComponent<Engine::Graphics::AnimationComponent>()) continue;
        if (!object->isActive()) continue;
        if (!object->hasComponent<Engine::Physics::PhysicsComponent>()) continue;
        if (!object->hasComponent<Engine::Core::TransformComponent>()) continue;
        if (!object->hasComponent<Engine::Graphics::SpriteComponent>()) continue;
        if (!object->hasComponent<Engine::Audio::SoundComponent>()) continue;
        if (!object->hasComponent<InvaderComponent>()) continue;


        Engine::Core::TransformComponent&     transform_component = object->getComponent<Engine::Core::TransformComponent>();
        Engine::Audio::SoundComponent&        sound_component     = object->getComponent<Engine::Audio::SoundComponent>();
        Engine::Graphics::AnimationComponent& animation_component = object->getComponent<Engine::Graphics::AnimationComponent>();
        InvaderComponent&                     invader_component   = object->getComponent<InvaderComponent>();
        AIComponent&                          AI_component        = object->getComponent<AIComponent>();


        if (AI_component.stateTimer < AI_component.stateLimit) {

            AI_component.stateTimer += deltaTime;
            continue;
        }

        AI_component.stateTimer = 0.0f;
        AI_component.stateRepetition++;

        engine_event_bus.publish<Engine::Events::PlaySoundEvent>("fastinvader1");


        float speed = 20.0f;
        if (AI_component.stateRepetition == 7) {

            AI_component.stateRepetition = 0;
            AI_component.currentState *= -1;

            animation_component.current_duration = 0.0f;
            animation_component.speed += 0.05f;

            AI_component.stateLimit = 1 / animation_component.speed;

            sound_component.previous_state = -1;

            transform_component.position += glm::vec2(0, speed);
        } else
            transform_component.position += glm::vec2(speed * AI_component.currentState, 0);


        float rand = Engine::Core::Random::getFloat(1.0f, 15.0f);
        if (rand > 1.1f) continue;


        Core::EmittorState emittor_state;
        switch (invader_component.type) {

            case Core::InvaderType::INVADER_A:
                emittor_state = Core::EmittorState::INVADER_A;
                break;
            case Core::InvaderType::INVADER_B:
                emittor_state = Core::EmittorState::INVADER_B;
                break;
            case Core::InvaderType::INVADER_C:
                emittor_state = Core::EmittorState::INVADER_C;
                break;
            default:
                emittor_state = Core::EmittorState::INVADER_A;
                break;
        }

        game_event_bus.publish<Events::EmitterEvent>(static_cast<int>(emittor_state), Engine::Physics::CollisionLayer::INVADER_BULLET,
                                                     Engine::Physics::CollisionLayer::PLAYER | Engine::Physics::CollisionLayer::WALL,
                                                     transform_component.position, glm::vec2{0.0f, 5.0f});
    }
}
}  // namespace Game::Control