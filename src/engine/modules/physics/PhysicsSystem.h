//
// Created by marcel on 11/9/25.
//

#pragma once
#include <vector>
#include <memory>

class GameObject;

class PhysicsSystem {

public:
                            PhysicsSystem()  = default;
                            ~PhysicsSystem() = default;

    void                    update(float deltatime,  std::vector<std::unique_ptr<GameObject> > &game_objects);
};