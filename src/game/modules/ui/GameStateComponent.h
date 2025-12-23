//
// Created by marcel on 12/6/25.
//

#pragma once

#include "engine/core/BaseComponent.h"

namespace Game::UI {
struct GameStateComponent : public Engine::Core::BaseComponent {
    int score        = 0;
    int lives        = 3;
    int num_invaders = 0;

    GameStateComponent(int score, int lives, int num_invaders) : score(score), lives(lives), num_invaders(num_invaders) {}
};
}  // namespace Game::UI