//
// Created by marcel on 9/28/25.
//

#pragma once

#include <memory>

class InputManager;
class Engine;
class RenderSystem;



class IGameState {
public:
    virtual                                 ~IGameState() = default;

    virtual void                            onEnter() {}
    virtual void                            onExit()  {}

    virtual std::unique_ptr<IGameState>     handleInput(InputManager &input_manager)    = 0;
    virtual std::unique_ptr<IGameState>     update(float deltatime, Engine& engine)     = 0;
    virtual void                            render(RenderSystem& renderer)              = 0;
};
