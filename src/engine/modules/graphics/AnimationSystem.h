//
// Created by marcel on 11/1/25.
//

#pragma once
#include <memory>
#include <vector>

namespace Engine::Assets { class AssetManager; }
namespace Engine::Core { class Context; }
namespace Engine::Graphics { struct AnimationComponent; struct SpriteComponent; }

namespace Engine::Graphics {
    class AnimationSystem {
    public:
        AnimationSystem()  = default;
        ~AnimationSystem() = default;

        void            update(Engine::Core::Context& ctx);
        SpriteComponent getSprite(AnimationComponent& anim);
    };
}  // namespace Engine::Graphics