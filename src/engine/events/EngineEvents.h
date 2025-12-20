//
// Created by marcel on 11/23/25.
//

#pragma once

#include "EventBus.h"
#include <string>

class GameObject;


struct PlaySoundEvent {

    std::string soundId;
};

struct CollisionEvent {

    GameObject *from;
    GameObject *to;
};

using EngineEventBus = EventBus<
    PlaySoundEvent,
    CollisionEvent
>;