//
// Created by marcel on 10/31/25.
//

#pragma once
#include <SDL_render.h>

namespace Engine::Assets {
    struct Texture {
        SDL_Texture* texture = nullptr;
        std::size_t  width;
        std::size_t  height;
    };
}  // namespace Engine::Assets