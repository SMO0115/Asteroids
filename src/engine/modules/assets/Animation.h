//
// Created by marcel on 10/31/25.
//

#pragma once

#include <vector>
#include "../../utils/Types.h"


struct Texture;

struct  Animation {

    Texture*                texture;
    std::vector<Rect>       frames;
    std::vector<float>      durations;
    bool                    loop;
};
