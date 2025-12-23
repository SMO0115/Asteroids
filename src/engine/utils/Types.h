//
// Created by marcel on 9/6/25.
//

#pragma once

#include <cstdint>

namespace Engine::Core {
struct Rect {
    int x, y, w, h;
};

struct Color {
    std::uint8_t r, g, b, a;
};
}  // namespace Engine::Core
