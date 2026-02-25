//
// Created by marcel on 11/1/25.
//

#include "engine/core/CoreModule.h"
#include "engine/modules/assets/AssetModule.h"

#include "AnimationComponent.h"
#include "AnimationSystem.h"
#include "SpriteComponent.h"


namespace Engine::Graphics {
void AnimationSystem::update(Engine::Core::Context& ctx) {

    float deltatime     = static_cast<float>(1.0f / 120.0f);
    auto  game_objects  = ctx.get<Engine::Core::Scene>().getAll();
    Engine::Assets::AssetManager&  asset_manager = ctx.get<Engine::Assets::AssetManager>();

    for (auto& object : game_objects) {

        if (!object.hasComponent<AnimationComponent>()) continue;
        if (!object.hasComponent<SpriteComponent>()) continue;

        AnimationComponent& anim   = object.getComponent<AnimationComponent>();
        SpriteComponent&    sprite = object.getComponent<SpriteComponent>();

        // 1. Safety Check
        if (anim.animations_ids.count(anim.current_state) == 0 || anim.animations_ids[anim.current_state] == 0) {
            sprite.texture_id = 0;
            continue;
        }

        // 2. Get the Asset
        auto current_anim = asset_manager.getAnimation(anim.animations_ids[anim.current_state]);

        // --- 3. THE "STATIC WALL" OPTIMIZATION ---
        // If speed is 0, we don't update timers or frames.
        // This saves CPU for your 64 walls!
        if (anim.speed > 0.0f) {
            anim.current_duration += deltatime * anim.speed;

            if (anim.current_duration > current_anim->durations[anim.current_frame]) {
                anim.current_duration = 0;
                anim.current_frame++;
                if (anim.current_frame >= current_anim->frames.size()) {
                    anim.current_frame = 0;
                }
            }
        }

        // 4. Update the Sprite (The "Animator drives Sprite" logic)
        // Refresh the asset pointer in case state changed
        current_anim = asset_manager.getAnimation(anim.animations_ids[anim.current_state]);

        sprite.texture_id = current_anim->texture_id;
        sprite.sourceRect = current_anim->frames[anim.current_frame];

        // // --- 5. THE "MATH HACK" INTEGRATION ---
        // // If Lua defined a custom rect (frame_override), use it!
        // // Otherwise, use the frame from the .json asset.
        // if (anim.has_custom_rect) {
        //     sprite.sourceRect = anim.custom_rect;
        // } else {
        //     sprite.sourceRect = current_anim->frames[anim.current_frame];
        // }
    }
}

SpriteComponent AnimationSystem::getSprite(AnimationComponent& anim) {

    SpriteComponent sprite;

    // sprite.texture_id = anim.animations[anim.current_state]->texture_id;
    // sprite.sourceRect = anim.animations[anim.current_state]->frames[anim.current_frame];

    return sprite;
}
}  // namespace Engine::Graphics