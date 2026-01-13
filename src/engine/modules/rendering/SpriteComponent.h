//
// Created by marcel on 9/30/25.
//

#pragma once

#include "../../core/components/BaseComponent.h"
#include "engine/utils/Types.h"

#include <string>

namespace Engine::Assets {
struct Texture;
}

namespace Engine::Graphics {
struct SpriteComponent : public Core::BaseComponent {
    std::string      texture_id;
    Assets::Texture* texture = nullptr;
    Core::Rect       sourceRect;
    Core::Color      color = {255, 255, 255, 255};

};
}  // namespace Engine::Graphics