//
// Created by marcel on 11/1/25.
//

#include "engine/core/CoreModule.h"
#include "engine/modules/assets/AssetModule.h"

#include "AnimationComponent.h"
#include "AnimationSystem.h"
#include "SpriteComponent.h"


namespace Engine::Graphics {
    void AnimationSystem::update(
        float delta,
        const std::vector<std::unique_ptr<Core::GameObject>>& game_objects,
        const Assets::AssetManager& asset_manager
    ) {

        for (const auto& object : game_objects) {

            if (!object->hasComponent<AnimationComponent>()) continue;
            if (!object->hasComponent<SpriteComponent>()) continue;

            AnimationComponent& anim   = object->getComponent<AnimationComponent>();
            SpriteComponent&    sprite = object->getComponent<SpriteComponent>();


            if (anim.animations_ids[anim.current_state] == 0) {

                sprite.texture_id = 0;
                continue;
            }


            anim.current_duration += delta * anim.speed;
            auto current_anim = asset_manager.getAnimation( anim.animations_ids[anim.current_state] );
            if (anim.current_duration > current_anim->durations[anim.current_frame]) {

                anim.current_duration = 0;
                anim.current_frame++;

                if (anim.current_frame >= current_anim->frames.size()) anim.current_frame = 0;
            }

            current_anim = asset_manager.getAnimation( anim.animations_ids[anim.current_state] );
            sprite.texture_id = current_anim->texture_id;
            sprite.sourceRect = current_anim->frames[anim.current_frame];
        }
    }

    SpriteComponent AnimationSystem::getSprite(AnimationComponent& anim) {

        SpriteComponent sprite;

        // sprite.texture_id = anim.animations[anim.current_state]->texture_id;
        // sprite.sourceRect = anim.animations[anim.current_state]->frames[anim.current_frame];

        return sprite;
    }
}  // namespace Engine::Graphics