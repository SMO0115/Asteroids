//
// Created by marcel on 11/16/25.
//

#pragma once
#include "glm/glm.hpp"

#include "../core/GameTypes.h"
#include "../../engine/events/EventBus.h"

class GameObject;


struct EmitterEvent {

    int             emitter_animation_id;
    CollisionLayer  layer;
    CollisionLayer  mask;
    glm::vec2       position;
    glm::vec2       velocity;
};

struct DeathEvent {
    GameObject*     object;
};



using GameEventBus = EventBus<
    EmitterEvent,
    DeathEvent
>;