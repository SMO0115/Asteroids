//
// Created by marcel on 9/30/25.
//

#pragma once

#include "engine/core/BaseComponent.h"
#include "engine/utils/Types.h"

namespace Engine::Assets { struct Texture; }

namespace Engine::Graphics {
    struct SpriteComponent : public Core::BaseComponent {

        Assets::Texture*    texture;
        Core::Rect          sourceRect;
        Core::Color         color = {255, 255, 255, 255};
    };
}