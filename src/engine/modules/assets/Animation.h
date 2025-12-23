//
// Created by marcel on 10/31/25.
//

#pragma once

#include <vector>
#include "engine/utils/Types.h"

namespace Engine::Assets {
struct Texture;

struct Animation {
    Texture*                texture;
    std::vector<Core::Rect> frames;
    std::vector<float>      durations;
    bool                    loop;
};
}  // namespace Engine::Assets