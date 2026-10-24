//
// Created by marcel on 8/28/25.
//

#include "InputManager.h"


namespace Engine::Input {

void InputManager::processInput() {

    m_pressed.fill(false);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT) {
            m_held[index_(Key::QUIT)]    = true;
            m_pressed[index_(Key::QUIT)] = true;
        }


        else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            Key mapped_key = mapKey_(event.key.keysym.sym);
            if (mapped_key == Key::UNKNOWN) continue;

            // Record the press separately, so a tap released before the next poll still counts.
            // Ignore OS key-repeat: holding a key is not a new press.
            if (event.type == SDL_KEYDOWN && event.key.repeat == 0) m_pressed[index_(mapped_key)] = true;

            m_held[index_(mapped_key)] = (event.type == SDL_KEYDOWN);
        }
    }
}

bool InputManager::wasKeyPressed(Key key) const { return m_pressed[index_(key)]; }

bool InputManager::isKeyHeld(Key key) const { return m_held[index_(key)]; }

Key InputManager::mapKey_(SDL_Keycode sdl_key) {

    switch (sdl_key) {
        case SDLK_UP:
            return Key::UP;
        case SDLK_DOWN:
            return Key::DOWN;
        case SDLK_LEFT:
            return Key::LEFT;
        case SDLK_RIGHT:
            return Key::RIGHT;
        case SDLK_SPACE:
            return Key::SPACE;
        case SDLK_RETURN:
        case SDLK_KP_ENTER:  // the keypad's Enter is a different key code
            return Key::ENTER;
        case SDLK_ESCAPE:
            return Key::ESCAPE;
        default:
            return Key::UNKNOWN;
    }
}
}  // namespace Engine::Input