//
// Created by marcel on 8/22/25.
//

#pragma once

class InputManager;
class Engine;
class RenderSystem;


class IGame {

public:
    virtual                             ~IGame()                                    = default;

    virtual bool                        init(Engine& engine)                        = 0;
    virtual void                        handleInput(InputManager &input_manager)    = 0;
    virtual bool                        update(float deltatime, Engine& engine)     = 0;
    virtual void                        render(RenderSystem& renderer)              = 0;
};