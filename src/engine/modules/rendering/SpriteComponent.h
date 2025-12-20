//
// Created by marcel on 9/30/25.
//

#pragma once

#include "engine/core/BaseComponent.h"

#include "../../utils/Types.h"

struct Texture;

struct SpriteComponent : public BaseComponent {

    Texture*    texture;
    Rect        sourceRect;

    Color       color = {255, 255, 255, 255};
};
