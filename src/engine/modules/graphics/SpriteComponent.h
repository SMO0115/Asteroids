//
// Created by marcel on 9/30/25.
//

#pragma once

#include "../../core/components/BaseComponent.h"
#include "engine/utils/Types.h"


namespace Engine::Graphics {
    struct SpriteComponent : public Core::BaseComponent {

        Assets::AssetID     texture_id = 0;

        Core::Rect          sourceRect = {0, 0, 0, 0};
        Core::Color         color = {255, 255, 255, 255};

    };
}  // namespace Engine::Graphics