#pragma once
#include "SDL_mixer.h"

struct Mix_Chunk;

namespace Engine::Assets {
struct Sound {
    ~Sound() {
        if (m_sdl_chunk) {
            Mix_FreeChunk(m_sdl_chunk);
        }
    }

    Mix_Chunk* m_sdl_chunk = nullptr;
};
}  // namespace Engine::Assets