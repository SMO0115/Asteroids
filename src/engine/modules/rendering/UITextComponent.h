//
// Created by marcel on 12/7/25.
//

#pragma once

#include <string>
#include "../../utils/Types.h"

#include "engine/core/BaseComponent.h"

struct Font;

struct UITextComponent : public BaseComponent {
    std::string     text;
    Font*           font        = nullptr;
    Color           color;
    bool            is_dirty    = true;

    int             x_offset;
    int             y_offset;

    UITextComponent(std::string t, Font* f, Color c = {255, 255, 255 }, int x=0, int y=0)
        : text(t), font(f), color(c), x_offset(x), y_offset(y) {}
};
