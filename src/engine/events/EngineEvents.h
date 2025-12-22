//
// Created by marcel on 11/23/25.
//

#pragma once

#include "EventBus.h"
#include <string>


namespace Engine::Core { class GameObject; }


namespace Engine::Events {

    struct PlaySoundEvent {

        std::string soundId;
    };

    struct CollisionEvent {

        Core::GameObject *from;
        Core::GameObject *to;
    };

    using EngineEventBus = EventBus<
        PlaySoundEvent,
        CollisionEvent
    >;
}