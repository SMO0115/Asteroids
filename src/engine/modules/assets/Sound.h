#pragma once
#include <memory>
#include "SDL_mixer.h"

namespace Engine::Assets {
// Same pattern as Font. The hand-written destructor this replaces had no deleted copy,
// so copying a Sound would have freed the chunk twice; unique_ptr can't be copied at all.
struct ChunkDeleter {
    void operator()(Mix_Chunk* chunk) const {
        if (chunk) Mix_FreeChunk(chunk);
    }
};

struct Sound {
    std::unique_ptr<Mix_Chunk, ChunkDeleter> m_sdl_chunk;
};
}  // namespace Engine::Assets
