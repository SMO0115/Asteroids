//
// Created by marcel on 11/22/25.
//

#pragma once

#include "engine/core/CoreModule.h"
#include "game/core/GameTypes.h"

namespace Game::Control {
struct InvaderComponent : public Engine::Core::BaseComponent {
    Core::InvaderType type;
    int               score_value = 10;
};
}  // namespace Game::Control