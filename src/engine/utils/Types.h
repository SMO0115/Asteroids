//
// Created by marcel on 9/6/25.
//

#pragma once

#include <cstdint>

namespace Engine::Core {
    struct Rect {
        int x = 0, y = 0, w = 0, h = 0;
    };

    // a defaults to 255: with plain 'int a;', Color{255, 255, 255} meant *invisible* white
    struct Color {
        int r = 0, g = 0, b = 0, a = 255;
    };
}


namespace Engine::Assets {
    
    // The Handle Type
    using AssetID = std::uint32_t;
    constexpr AssetID INVALID_ID = 0; // 0 is reserved for "null/missing"
}


using State = int;