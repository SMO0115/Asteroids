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
        int r, g, b, a;
    };
}


namespace Engine::Assets {
    
    // The Handle Type
    using AssetID = std::uint32_t;
    constexpr AssetID INVALID_ID = 0; // 0 is reserved for "null/missing"
}


using State = int;