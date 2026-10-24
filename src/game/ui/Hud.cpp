//
// Created by smo0115 on 10/24/26.
//

#include "Hud.h"

#include <string>

#include "Text.h"
#include "engine/core/CoreModule.h"
#include "engine/modules/assets/AssetModule.h"
#include "engine/modules/graphics/RenderModule.h"
#include "game/core/GameSession.h"
#include "game/core/HighScore.h"

namespace Game::UI {

constexpr int       FONT_SIZE  = SMALL_TEXT;
constexpr int       MARGIN     = 20;              // px from the screen's top-left corner
constexpr float     ICON_SCALE = 0.5f;            // the life icons are half-size ships ...
constexpr float     ICON_STEP  = 36.0f;           // ... this far apart (px) ...
constexpr glm::vec2 ICON_START = {36.0f, 80.0f};  // ... starting under the score


Hud::Hud() : m_score("0", "pixel", Engine::Core::Color{255, 255, 255, 255}, MARGIN, MARGIN / 2) { m_score.size = FONT_SIZE; }


void Hud::render(Engine::Core::Context& ctx) {

    Engine::Graphics::RenderSystem& renderer      = ctx.get<Engine::Graphics::RenderSystem>();
    Engine::Assets::AssetManager&   asset_manager = ctx.get<Engine::Assets::AssetManager>();
    const Game::Core::GameSession&  session       = ctx.get<Game::Core::GameSession>();

    // 1. Score. Setting the text every frame is cheap: the RenderSystem only renders it again when it
    // changed (its text cache)
    m_score.text    = std::to_string(session.score);
    m_score.font_id = asset_manager.getFontID(m_score.font_name, FONT_SIZE);
    renderer.drawText(m_score, asset_manager);

    // The high score, top center like the arcade: the best of the games BEFORE this one, and nothing
    // until there is one. (It used to show max(best, score), so in a first game, or after beating the
    // record, the same number was on screen twice.)
    const int best = ctx.get<Game::Core::HighScore>().best();
    if (best > 0) drawCenteredText(ctx, std::to_string(best), FONT_SIZE, MARGIN / 2);

    // 2. Lives, as small ships pointing up: the first frame of the ship's idle animation
    const Engine::Assets::Animation* ship = asset_manager.getAnimation(asset_manager.getAnimationID("player_idle"));
    if (ship == nullptr || ship->frames.empty()) return;

    Engine::Graphics::SpriteComponent icon;
    icon.texture_id = ship->texture_id;
    icon.sourceRect = ship->frames[0];

    Engine::Core::TransformComponent place;
    place.scale    = ICON_SCALE;
    place.rotation = -90.0f;

    for (int i = 0; i < session.lives; ++i) {
        place.position = ICON_START + glm::vec2(static_cast<float>(i) * ICON_STEP, 0.0f);
        renderer.draw(icon, place, asset_manager);
    }
}
}  // namespace Game::UI
