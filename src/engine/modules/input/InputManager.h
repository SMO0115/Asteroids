//
// Created by marcel on 8/28/25.
//

#pragma once

#include <SDL_keycode.h>
#include <array>
#include <cstddef>

namespace Engine::Input {
enum class Key { UNKNOWN, UP, DOWN, LEFT, RIGHT, W, A, S, D, SPACE, ENTER, ESCAPE, QUIT, COUNT };

class InputManager {
public:
    InputManager()  = default;
    ~InputManager() = default;

    void processInput();
    bool wasKeyPressed(Key key) const;
    bool isKeyHeld(Key key) const;

private:
    Key mapKey_(SDL_Keycode sdl_key);

    static constexpr std::size_t index_(Key key) { return static_cast<std::size_t>(key); }

private:
    static constexpr std::size_t KEY_COUNT = static_cast<std::size_t>(Key::COUNT);

    std::array<bool, KEY_COUNT> m_held{};     // State: is the key down right now
    std::array<bool, KEY_COUNT> m_pressed{};  // Event: went down since the last processInput()
};
}  // namespace Engine::Input
