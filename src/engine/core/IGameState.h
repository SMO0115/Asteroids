//
// Created by marcel on 9/28/25.
//

#pragma once

#include <memory>

namespace Engine            { class Application; }
namespace Engine::Input     { class InputManager; }
namespace Engine::Graphics  { class RenderSystem; }



namespace Engine::Core {
    class IGameState {
    public:
        virtual                                 ~IGameState()                                   = default;

        virtual void                            onEnter() {}
        virtual void                            onExit()  {}

        virtual std::unique_ptr<IGameState>     handleInput(Input::InputManager &input_manager) = 0;
        virtual std::unique_ptr<IGameState>     update(float deltatime, Application& engine)    = 0;
        virtual void                            render(Graphics::RenderSystem& renderer)        = 0;
    };
}