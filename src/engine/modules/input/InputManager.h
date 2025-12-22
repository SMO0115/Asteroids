//
// Created by marcel on 8/28/25.
//

#pragma once

#include <SDL.h>
#include <unordered_map>



namespace Engine::Input {
    enum class Key {
        UNKNOWN,
        UP,
        DOWN,
        LEFT,
        RIGHT,
        W,
        A,
        S,
        D,
        SPACE,
        ESCAPE,
        QUIT
    };

    class InputManager {
    public:
        InputManager()  = default;
        ~InputManager() = default;


        void                                processInput();
        bool                                wasKeyPressed(Key key);
        bool                                isKeyHeld(Key key);

    private:
        Key                                 mapKey_(SDL_Keycode sdl_key);

    private:
        Key                                 m_last_key;
        std::unordered_map<Key, bool>       m_current_key_states;
        std::unordered_map<Key, bool>       m_previous_key_states;
    };
}


