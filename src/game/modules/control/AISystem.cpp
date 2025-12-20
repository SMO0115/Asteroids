//
// Created by marcel on 11/8/25.
//

#include "AISystem.h"

#include "engine/core/CoreModule.h"
#include "game/core/CoreModule.h"

#include "AIComponent.h"
#include "engine/utils/Random.h"


#include "engine/modules/physics/PhysicsModule.h"
#include "engine/modules/rendering/RenderModule.h"
#include "engine/modules/audio/AudioModule.h"



void AISystem::update(float deltaTime, EngineEventBus& engine_event_bus, GameEventBus& game_event_bus, std::vector<std::unique_ptr<GameObject> > &game_objects) {

    for (const auto &object : game_objects) {


        if (!object->hasComponent<AIComponent>()) continue;

        if ( !object->hasComponent<AnimationComponent>() ) continue;

        if (!object->isActive()) continue;
        if (!object->hasComponent<PhysicsComponent>()) continue;
        if (!object->hasComponent<TransformComponent>()) continue;
        if (!object->hasComponent<SpriteComponent>()) continue;
        if (!object->hasComponent<SoundComponent>()) continue;
        if (!object->hasComponent<InvaderComponent>()) continue;



        TransformComponent& transform_component = object->getComponent<TransformComponent>();
        AIComponent&        AI_component        = object->getComponent<AIComponent>();
        SoundComponent&     sound_component     = object->getComponent<SoundComponent>();
        AnimationComponent& animation_component = object->getComponent<AnimationComponent>();
        InvaderComponent&   invader_component   = object->getComponent<InvaderComponent>();


        if (AI_component.stateTimer < AI_component.stateLimit) {

            AI_component.stateTimer += deltaTime;
            continue;
        }

        AI_component.stateTimer = 0.0f;
        AI_component.stateRepetition++;

        engine_event_bus.publish<PlaySoundEvent>("fastinvader1");


        float speed = 20.0f;
        if (AI_component.stateRepetition == 7) {

            AI_component.stateRepetition = 0;
            AI_component.currentState *= -1;

            animation_component.current_duration = 0.0f;
            animation_component.speed += 0.05f;

            AI_component.stateLimit = 1 / animation_component.speed;

            sound_component.previous_state = -1;

            transform_component.position += glm::vec2(0, speed);
        }
        else transform_component.position += glm::vec2(speed  * AI_component.currentState, 0);



        float rand = Random::getFloat(1.0f, 15.0f);
        if (rand > 1.1f) continue;


        EmittorState emittor_state;
        switch (invader_component.type) {

            case InvaderType::INVADER_A: emittor_state = EmittorState::INVADER_A; break;
            case InvaderType::INVADER_B: emittor_state = EmittorState::INVADER_B; break;
            case InvaderType::INVADER_C: emittor_state = EmittorState::INVADER_C; break;
            default: emittor_state = EmittorState::INVADER_A; break;
        }

        game_event_bus.publish<EmitterEvent>(
            static_cast<int>( emittor_state ),
            CollisionLayer::INVADER_BULLET,
            CollisionLayer::PLAYER | CollisionLayer::WALL,
            transform_component.position, glm::vec2{0.0f, 5.0f}
        );

    }
}
