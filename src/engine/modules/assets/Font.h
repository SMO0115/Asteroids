//
// Created by marcel on 12/13/25.
//

#pragma once

#include <SDL2/SDL_ttf.h>
#include <memory>
#include <string>

namespace Engine::Assets {
struct FontDeleter {
    void operator()(TTF_Font* font) const {
        if (font) TTF_CloseFont(font);
    }
};

struct Font {
    std::unique_ptr<TTF_Font, FontDeleter> ttf_font;

    std::string file_path;
    std::size_t font_size;

    Font(TTF_Font* raw_font, const std::string& path, int size) : ttf_font(raw_font), file_path(path), font_size(size) {}

    TTF_Font* get() const { return ttf_font.get(); }
};
}  // namespace Engine::Assets