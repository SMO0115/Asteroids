//
// Created by marcel on 11/1/25.
//

#include "engine/core/CoreModule.h"
#include "engine/modules/assets/AssetModule.h"

#include "AnimationComponent.h"
#include "AnimationSystem.h"
#include "SpriteComponent.h"


namespace Engine::Graphics {
void AnimationSystem::update(Engine::Core::Context& ctx, float dt) {

    auto                          game_objects  = ctx.get<Engine::Core::Scene>().getAll();
    Engine::Assets::AssetManager& asset_manager = ctx.get<Engine::Assets::AssetManager>();

    for (auto& object : game_objects) {

        if (!object.isActive()) continue;  // not drawn anyway
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
        if (current_anim == nullptr || current_anim->frames.empty()) {
            sprite.texture_id = 0;
            continue;
        }

        // A new state starts its animation from the first frame. Without this, current_frame kept its
        // old value, which is past the end of the new animation if that one has fewer frames.
        if (anim.current_state != anim.previous_state) {
            anim.previous_state   = anim.current_state;
            anim.current_frame    = 0;
            anim.current_duration = 0.0f;
        }

        // --- 3. THE "STATIC WALL" OPTIMIZATION ---
        // If speed is 0, we don't update timers or frames.
        // This saves CPU for your 64 walls!
        if (anim.speed > 0.0f) {
            anim.current_duration += dt * anim.speed;

            const float frame_duration = current_anim->durations[anim.current_frame];
            if (anim.current_duration >= frame_duration) {
                anim.current_duration -= frame_duration;  // keep the leftover time, so the speed stays exact

                if (anim.current_frame + 1 < current_anim->frames.size()) {
                    anim.current_frame++;
                } else if (current_anim->loop) {
                    anim.current_frame = 0;
                }  // else: a one-shot animation (e.g. an explosion) stays on its last frame
            }
        }

        // 4. Update the Sprite (The "Animator drives Sprite" logic)
        sprite.texture_id = current_anim->texture_id;
        sprite.sourceRect = current_anim->frames[anim.current_frame];
    }
}
}  // namespace Engine::Graphics