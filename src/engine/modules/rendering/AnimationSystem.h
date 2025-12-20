//
// Created by marcel on 11/1/25.
//

#pragma once
#include <memory>


struct AnimationComponent;
struct SpriteComponent;


class GameObject;

class AnimationSystem {
public:
                                    AnimationSystem()  = default;
                                    ~AnimationSystem() = default;

    void                            update(float delta, const std::vector<std::unique_ptr<GameObject>>& game_objects);
    SpriteComponent                 getSprite(AnimationComponent& anim);
};
