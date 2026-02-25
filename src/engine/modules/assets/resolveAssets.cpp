// resolveAssets.cpp
#include "resolveAssets.h"
#include "engine/core/CoreModule.h"
#include "engine/modules/assets/AssetManager.h"

namespace Engine::Assets {

void resolveAssets(Engine::Core::Context& ctx) {

    auto& asset_manager = ctx.get<AssetManager>();
    auto& scene         = ctx.get<Core::Scene>();

    // ------------------------------------------------------------
    // CHANGE 1: Use the flattened view
    // 'getAll()' automatically iterates through every pool (Enemies, UI, etc.)
    // ------------------------------------------------------------
    for (auto& obj : scene.getAll()) {

        // ------------------------------------------------------------
        // CHANGE 2: 'obj' is a reference (GameObject&), use '.' not '->'
        // ------------------------------------------------------------

        // 1. Resolve Fonts
        if (obj.hasComponent<Graphics::UITextComponent>()) {
            auto& text = obj.getComponent<Graphics::UITextComponent>();

            // Optimized: Only resolve if ID is missing or dirty (optional check)
            text.font_id = asset_manager.getFontID(text.font_name, text.size);
        }

        // 2. Resolve Animations
        if (obj.hasComponent<Graphics::AnimationComponent>()) {
            auto& animation = obj.getComponent<Graphics::AnimationComponent>();

            for (const auto& [key, anim_name] : animation.animations) {
                animation.animations_ids[key] = asset_manager.getAnimationID(anim_name);
            }
        }

        // 3. Resolve Sounds
        if (obj.hasComponent<Audio::SoundComponent>()) {
            auto& sound = obj.getComponent<Audio::SoundComponent>();

            for (const auto& [key, audio] : sound.sounds) {
                sound.sounds_ids[key] = asset_manager.getSoundID(audio);
            }
        }
    }
}

}  // namespace Engine::Assets