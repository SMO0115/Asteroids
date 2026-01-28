//
// Created by marcel on 11/1/25.
//

#pragma once
#include <memory>
#include <vector>

namespace Engine::Assets { class AssetManager; }

namespace Engine::Core {
    class GameObject;
    }

    namespace Engine::Graphics {

    struct AnimationComponent;
    struct SpriteComponent;

    class AnimationSystem {
    public:
        AnimationSystem()  = default;
        ~AnimationSystem() = default;

        void            update(float delta, const std::vector<std::unique_ptr<Core::GameObject>>& game_objects, const Assets::AssetManager& asset_manager);
        SpriteComponent getSprite(AnimationComponent& anim);
    };
}  // namespace Engine::Graphics