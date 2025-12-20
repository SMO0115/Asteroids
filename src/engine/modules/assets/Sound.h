#pragma once
#include "SDL_mixer.h"


struct Mix_Chunk;

struct Sound {
    ~Sound() {
        if (m_sdl_chunk) {
            Mix_FreeChunk(m_sdl_chunk);
        }
    }
    
private:
    friend class SoundSystem;
    friend class AssetManager;

    Mix_Chunk* m_sdl_chunk = nullptr;
};