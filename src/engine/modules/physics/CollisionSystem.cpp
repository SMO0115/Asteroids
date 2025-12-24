//
// Created by marcel on 11/22/25.
//

#include <iostream>
#include "engine/core/CoreModule.h"


#include "ColliderComponent.h"
#include "CollisionSystem.h"

#include "engine/events/EventBus.h"


namespace Engine::Physics {

struct ColliderCache {
    Core::GameObject*         obj;
    ColliderComponent*        col;
    Core::TransformComponent* trans;
};

void CollisionSystem::update(Events::EngineEventBus& event_bus, const std::vector<std::unique_ptr<Core::GameObject>>& game_objects) {


    std::vector<ColliderCache> active_colliders;
    active_colliders.reserve(game_objects.size());  // Prevent allocations

    for (const auto& obj_ptr : game_objects) {
        Core::GameObject* obj = obj_ptr.get();

        // Early exit conditions
        if (!obj->isActive()) continue;

        // Check map ONCE per object, not N times
        if (obj->hasComponent<ColliderComponent>() && obj->hasComponent<Core::TransformComponent>()) {
            active_colliders.push_back({
                obj,
                &obj->getComponent<ColliderComponent>(),        // Store direct pointer
                &obj->getComponent<Core::TransformComponent>()  // Store direct pointer
            });
        }
    }


    size_t count = active_colliders.size();
    for (size_t i = 0; i < count; ++i) {

        ColliderCache& cache_a = active_colliders[i];

        for (size_t j = i + 1; j < count; ++j) {

            ColliderCache& cache_b = active_colliders[j];

            bool a_hits_b = static_cast<int>(cache_a.col->mask & cache_b.col->layer) != 0;
            bool b_hits_a = static_cast<int>(cache_b.col->mask & cache_a.col->layer) != 0;

            if (!a_hits_b && !b_hits_a) continue;

            if (checkCollisionAABB_(*cache_a.trans, *cache_b.trans, cache_a.col->bounds, cache_b.col->bounds)) {

                if (a_hits_b) event_bus.publish<Events::CollisionEvent>(cache_a.obj, cache_b.obj);
                if (b_hits_a) event_bus.publish<Events::CollisionEvent>(cache_b.obj, cache_a.obj);
            }
        }
    }
}


bool CollisionSystem::checkCollisionAABB_(const Core::TransformComponent& trans_a, const Core::TransformComponent& trans_b, const Core::Rect& obj_a,
                                          const Core::Rect& obj_b) {

    Core::Rect box_a = {
        static_cast<int>(trans_a.position.x),
        static_cast<int>(trans_a.position.y),
        static_cast<int>(trans_a.scale * obj_a.w),
        static_cast<int>(trans_a.scale * obj_a.h),
    };

    Core::Rect box_b = {
        static_cast<int>(trans_b.position.x),
        static_cast<int>(trans_b.position.y),
        static_cast<int>(trans_b.scale * obj_b.w),
        static_cast<int>(trans_b.scale * obj_b.h),
    };


    if (box_a.x - box_a.w / 2 < box_b.x + box_b.w / 2 && box_a.x + box_a.w / 2 > box_b.x - box_b.w / 2 && box_a.y - box_a.h / 2 < box_b.y + box_b.h / 2 &&
        box_a.y + box_a.h / 2 > box_b.y - box_b.h / 2)
        return true;

    return false;
}
}  // namespace Engine::Physics