//
// Created by marcel on 8/22/25.
//

#pragma once

namespace Engine::Input     { class InputManager;   }
namespace Engine::Core      { class Context;        }

namespace Engine::Core {
    class IGame {

    public:
        virtual                             ~IGame()                                                = default;

        virtual bool                        init(Engine::Core::Context& ctx)                        = 0;
        virtual void                        handleInput(Input::InputManager &input_manager)         = 0;
        virtual bool                        update(float deltatime, Engine::Core::Context& engine)  = 0;
        virtual void                        render(Engine::Core::Context& ctx)                      = 0;
    };
}