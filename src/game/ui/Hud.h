//
// Created by smo0115 on 10/24/26.
//

#pragma once

#include "engine/modules/graphics/UITextComponent.h"

namespace Engine::Core { class Context; }

namespace Game::UI {

    // Score and remaining lives, drawn on top of the world. A Context service, so every state that
    // shows the world (run, pause, game over) shows the HUD too.
    class Hud {
    public:
        Hud();
        ~Hud() = default;

        void render(Engine::Core::Context& ctx);

    private:
        // Just the component, without a GameObject: the RenderSystem can draw a UITextComponent directly
        Engine::Graphics::UITextComponent m_score;
    };
}  // namespace Game::UI
