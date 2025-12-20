//
// Created by marcel on 8/28/25.
//

#include "InputManager.h"


void InputManager::processInput() {

    m_previous_key_states = m_current_key_states;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT) m_current_key_states[Key::QUIT] = true;


        else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            Key mapped_key = mapKey_(event.key.keysym.sym);
            if (mapped_key != Key::UNKNOWN) {
                m_current_key_states[mapped_key] = (event.type == SDL_KEYDOWN);
            }
        }

    }
}

bool InputManager::wasKeyPressed(Key key) {
    return m_current_key_states[key] && !m_previous_key_states[key];
}

bool InputManager::isKeyHeld(Key key) {
    return m_current_key_states[key];
}

Key InputManager::mapKey_(SDL_Keycode sdl_key) {

    switch (sdl_key) {
        case SDLK_UP:           return Key::UP;
        case SDLK_DOWN:         return Key::DOWN;
        case SDLK_LEFT:         return Key::LEFT;
        case SDLK_RIGHT:        return Key::RIGHT;
        case SDLK_SPACE:        return Key::SPACE;
        case SDLK_ESCAPE:       return Key::ESCAPE;
        default:                return Key::UNKNOWN;
    }
}
