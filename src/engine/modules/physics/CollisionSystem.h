//
// Created by marcel on 11/22/25.
//

#pragma once

#include <vector>
#include <memory>
#include "../../events/EngineEvents.h"

class GameObject;
class TransformComponent;
struct Rect;


class CollisionSystem {

public:

                                        CollisionSystem()  = default;
                                        ~CollisionSystem() = default;

    void                                update(
                                            EngineEventBus&                                   event_bus,
                                            const std::vector< std::unique_ptr<GameObject> >& game_objects
                                        );

private:

    bool                                checkCollisionAABB_(
                                            const TransformComponent& trans_a,
                                            const TransformComponent& trans_b,
                                            const Rect&                 obj_a,
                                            const Rect&                 obj_b
                                        );
};
