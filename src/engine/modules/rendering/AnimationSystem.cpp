//
// Created by marcel on 11/1/25.
//

#include "engine/core/CoreModule.h"
#include "engine/modules/assets/AssetModule.h"

#include "AnimationSystem.h"
#include "AnimationComponent.h"
#include "SpriteComponent.h"



namespace Engine::Graphics {
    void AnimationSystem::update(float delta, const std::vector<std::unique_ptr<Core::GameObject>>& game_objects) {

        for (const auto &object : game_objects) {


            if (!object->hasComponent<AnimationComponent>()) continue;
            if (!object->hasComponent<SpriteComponent>()) continue;

            AnimationComponent& anim = object->getComponent<AnimationComponent>();
            SpriteComponent& sprite = object->getComponent<SpriteComponent>();


            if ( anim.animations[anim.current_state] == nullptr ) {

                sprite.texture = nullptr;
                continue;
            }


            anim.current_duration += delta * anim.speed;
            if (anim.current_duration > anim.animations[anim.current_state]->durations[anim.current_frame]) {

                anim.current_duration = 0;
                anim.current_frame++;

                if (anim.current_frame >= anim.animations[anim.current_state]->frames.size()) anim.current_frame = 0;
            }

            sprite.texture = anim.animations[anim.current_state]->texture;
            sprite.sourceRect = anim.animations[anim.current_state]->frames[anim.current_frame];
        }
    }

    SpriteComponent AnimationSystem::getSprite(AnimationComponent& anim) {

        SpriteComponent sprite;
        sprite.texture = anim.animations[anim.current_state]->texture;
        sprite.sourceRect = anim.animations[anim.current_state]->frames[anim.current_frame];

        return sprite;
    }
}