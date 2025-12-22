//
// Created by marcel on 8/22/25.
//

#pragma once

namespace Engine            { class Application; }
namespace Engine::Input     { class InputManager; }
namespace Engine::Graphics  { class RenderSystem; }


namespace Engine::Core {
    class IGame {

    public:
        virtual                             ~IGame()                                        = default;

        virtual bool                        init(Application& engine)                       = 0;
        virtual void                        handleInput(Input::InputManager &input_manager) = 0;
        virtual bool                        update(float deltatime, Application& engine)    = 0;
        virtual void                        render(Graphics::RenderSystem& renderer)        = 0;
    };
}