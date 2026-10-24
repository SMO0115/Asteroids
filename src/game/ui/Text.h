//
// Created by smo0115 on 10/24/26.
//

#pragma once

#include <string>

namespace Engine::Core { class Context; }

namespace Game::UI {

    // The two sizes of the "pixel" font that asset_loader.lua loads
    constexpr int BIG_TEXT   = 72;
    constexpr int SMALL_TEXT = 32;

    // Draws 'text' in the "pixel" font, centered left-right, with its top at 'y' (px).
    // The menu, pause and game-over screens all need this, so it's written once.
    void drawCenteredText(Engine::Core::Context& ctx, const std::string& text, int font_size, int y);
}  // namespace Game::UI
