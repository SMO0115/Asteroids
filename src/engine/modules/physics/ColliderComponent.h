//
// Created by marcel on 9/29/25.
//

#pragma once

#include "engine/core/BaseComponent.h"
#include "game/core/GameTypes.h"
#include "../../utils/Types.h"

class GameObject;
struct CollisionInfo;



struct ColliderComponent : public BaseComponent {

    CollisionLayer      layer   = CollisionLayer::NONE;
    CollisionLayer      mask    = CollisionLayer::NONE;

    Rect                bounds  = {0, 0, 0, 0};
};