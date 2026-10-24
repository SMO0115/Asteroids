//
// Created by marcel on 9/28/25.
//

#pragma once

#include <memory>

namespace Engine            { class Application;  }
namespace Engine::Input     { class InputManager; }
namespace Engine::Graphics  { class RenderSystem; }
namespace Engine::Core      { class Context;      }


namespace Engine::Core {
    class IGameState {
    public:
        virtual                                 ~IGameState()                                   = default;

        // They get the Context, so a state can e.g. pause the audio when it starts
        virtual void                            onEnter(Engine::Core::Context& /*ctx*/) {}
        virtual void                            onExit(Engine::Core::Context& /*ctx*/)  {}

        virtual std::unique_ptr<IGameState>     handleInput(Engine::Core::Context& ctx)  = 0;
        virtual std::unique_ptr<IGameState>     update(float deltatime, Context& ctx)    = 0;
        virtual void                            render(Engine::Core::Context& ctx)       = 0;
    };
}