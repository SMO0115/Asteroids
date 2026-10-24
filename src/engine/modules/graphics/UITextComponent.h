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

        std::string     font_name = "default";
        Assets::AssetID font_id   = 0;
        std::string     text      = "";
        Core::Color     color     =  {255, 255, 255, 255};
        std::size_t     size      =  0;  // the font size: font_name + size -> font_id (resolveAssets)

        int x_offset = 0;
        int y_offset = 0;

        // Members are initialized in DECLARATION order, so list them in that order (-Wreorder).
        // Parameters get their own names so they don't shadow the members (-Wshadow), and the
        // strings are taken by value and moved in: one copy instead of two.
        UITextComponent(std::string text_value, std::string font, Core::Color text_color = {255, 255, 255, 255}, int x = 0, int y = 0)
            : font_name(std::move(font)), text(std::move(text_value)), color(text_color), x_offset(x), y_offset(y) {}

        UITextComponent() = default;
    };
}  // namespace Engine::Graphics