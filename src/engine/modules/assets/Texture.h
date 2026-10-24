//
// Created by marcel on 10/31/25.
//

#pragma once
#include <SDL_render.h>
#include <cstddef>
#include <memory>

namespace Engine::Assets {
    // Same pattern as Font: unique_ptr + deleter frees the SDL texture automatically
    struct TextureDeleter {
        void operator()(SDL_Texture* texture) const {
            if (texture) SDL_DestroyTexture(texture);
        }
    };

    struct Texture {
        std::unique_ptr<SDL_Texture, TextureDeleter> texture;
        std::size_t                                  width  = 0;
        std::size_t                                  height = 0;
    };
}  // namespace Engine::Assets
