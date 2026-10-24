//
// Created by smo0115 on 10/24/26.
//

#include "Text.h"

#include "engine/core/CoreModule.h"
#include "engine/modules/assets/AssetModule.h"
#include "engine/modules/graphics/RenderModule.h"

namespace Game::UI {

void drawCenteredText(Engine::Core::Context& ctx, const std::string& text, int font_size, int y) {

    Engine::Graphics::RenderSystem& renderer      = ctx.get<Engine::Graphics::RenderSystem>();
    Engine::Assets::AssetManager&   asset_manager = ctx.get<Engine::Assets::AssetManager>();

    Engine::Graphics::UITextComponent label(text, "pixel");
    label.size    = static_cast<std::size_t>(font_size);
    label.font_id = asset_manager.getFontID(label.font_name, font_size);

    const Engine::Assets::Font* font = asset_manager.getFont(label.font_id);
    if (font == nullptr) return;  // font not loaded: nothing we could draw

    // Center using the text's real size, so it fits any screen size and any font
    int width = 0, height = 0;
    renderer.getTextSize(text, &width, &height, font->get());
    label.x_offset = (renderer.getScreenSize().x - width) / 2;
    label.y_offset = y;

    renderer.drawText(label, asset_manager);
}
}  // namespace Game::UI
