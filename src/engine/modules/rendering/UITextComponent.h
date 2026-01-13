//
// Created by marcel on 12/7/25.
//

#pragma once

#include <string>
#include <utility>
#include "engine/core/components/BaseComponent.h"
#include "engine/utils/Types.h"

namespace Engine::Assets {
struct Font;
}

namespace Engine::Graphics {
    struct UITextComponent : public Core::BaseComponent {

        std::string   font_id   = "";
        Assets::Font* font      = nullptr;
        std::string   text      = "";
        Core::Color   color     =  {255, 255, 255, 255};
        bool          is_dirty  = true;

        int x_offset = 0;
        int y_offset = 0;

        UITextComponent(std::string text, Assets::Font* font, Core::Color color = {255, 255, 255 }, int x=0, int y=0)
            : text(text), font(font), color(color), x_offset(x), y_offset(y) {}

        UITextComponent();
    };
}  // namespace Engine::Graphics