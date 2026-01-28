//
// Created by marcel on 10/31/25.
//

#pragma once

#include <vector>
#include "engine/utils/Types.h"


namespace Engine::Assets {

    struct Animation {
        AssetID                 texture_id;
        std::vector<Core::Rect> frames;
        std::vector<float>      durations;
        bool                    loop;
    };
}  // namespace Engine::Assets