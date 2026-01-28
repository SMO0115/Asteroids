//
// Created by marcel on 1/25/26.
//

#include "resolveAssets.h"
#include "engine/core/CoreModule.h"
#include "engine/modules/assets/AssetManager.h"

namespace Engine::Assets {

    void resolveAssets(Engine::Application& ctx) {

        AssetManager& asset_manager = ctx.getAssetManager();
        Core::Scene& scene = ctx.getScene();

        for (auto& [name, pool] : scene.getAllPools()) {

            for (auto& obj : pool) {

                if (obj->hasComponent<Graphics::UITextComponent>()) {

                    Graphics::UITextComponent& text = obj->getComponent<Graphics::UITextComponent>();
                    text.font_id = asset_manager.getFontID(text.font_name, text.size);
                }

                if (obj->hasComponent<Graphics::AnimationComponent>()) {
                    Graphics::AnimationComponent& animation = obj->getComponent<Graphics::AnimationComponent>();

                    for (const auto& [key, anim_name] : animation.animations) {

                        animation.animations_ids[key] = asset_manager.getAnimationID(anim_name);
                    }
                }

                if (obj->hasComponent<Audio::SoundComponent>()) {

                    Audio::SoundComponent& sound = obj->getComponent<Audio::SoundComponent>();

                    for (const auto& [key, audio] : sound.sounds) {
                        sound.sounds_ids[key] = asset_manager.getSoundID(audio);
                    }
                }

            }
        }

    }
}