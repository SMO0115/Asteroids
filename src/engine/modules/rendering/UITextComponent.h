//
// Created by marcel on 12/7/25.
//

#pragma once

#include <string>
#include <utility>
#include "engine/core/BaseComponent.h"
#include "engine/utils/Types.h"


namespace Engine::Assets { struct Font; }

namespace Engine::Graphics {
    struct UITextComponent : public Core::BaseComponent {
        std::string     text;
        Assets::Font*   font        = nullptr;
        Core::Color     color;
        bool            is_dirty    = true;

        int             x_offset;
        int             y_offset;

        UITextComponent(std::string t, Assets::Font* f, Core::Color c = {255, 255, 255 }, int x=0, int y=0)
            : text(std::move(t)), font(f), color(c), x_offset(x), y_offset(y) {}
    };
}