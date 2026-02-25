//
// Created by marcel on 11/22/25.
//

#include "CollisionSystem.h"
#include "ColliderComponent.h"
#include "engine/core/CoreModule.h"
#include "engine/events/EventBus.h"

namespace Engine::Physics {

struct ColliderCache {
    Core::GameObject*         obj;
    ColliderComponent*        col;
    Core::TransformComponent* trans;
};

void CollisionSystem::update(Core::Context& ctx) {

    // CHANGE 1: Remove 'const' and '&'.
    // MultiPoolView is lightweight (just pointers) and intended to be used by value.
    auto game_objects = ctx.get<Engine::Core::Scene>().getAll();

    Events::EventBus& event_bus = ctx.get<Events::EventBus>();

    std::vector<ColliderCache> active_colliders;
    // Optional: Heuristic reserve (e.g., 100 objects) to save some allocations
    active_colliders.reserve(128);

    // CHANGE 2: Use 'auto&' (non-const) so we can take a mutable pointer '&obj'
    for (auto& obj : game_objects) {

        // Early exit conditions
        if (!obj.isActive()) continue;

        // Check map ONCE per object, not N times
        // Note: Since 'obj' is a reference, use '.' to access members
        if (obj.hasComponent<ColliderComponent>() && obj.hasComponent<Core::TransformComponent>()) {
            active_colliders.push_back({
                &obj,                                          // Now valid (mutable pointer)
                &obj.getComponent<ColliderComponent>(),        // Direct pointer
                &obj.getComponent<Core::TransformComponent>()  // Direct pointer
            });
        }
    }

    size_t count = active_colliders.size();
    for (size_t i = 0; i < count; ++i) {

        ColliderCache& cache_a = active_colliders[i];

        for (size_t j = i + 1; j < count; ++j) {

            ColliderCache& cache_b = active_colliders[j];

            // Optimization: Bitwise check is extremely fast, do it first
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

    // CHANGE 3: Apply the collider's local offset (obj_a.x / obj_a.y)
    // If your collider is defined relative to the center, you MUST add this offset.


    Core::Rect box_a = {
        static_cast<int>(trans_a.position.x + (obj_a.x * trans_a.scale)),  // Apply Offset X
        static_cast<int>(trans_a.position.y + (obj_a.y * trans_a.scale)),  // Apply Offset Y
        static_cast<int>(trans_a.scale * obj_a.w),
        static_cast<int>(trans_a.scale * obj_a.h),
    };

    Core::Rect box_b = {
        static_cast<int>(trans_b.position.x + (obj_b.x * trans_b.scale)),  // Apply Offset X
        static_cast<int>(trans_b.position.y + (obj_b.y * trans_b.scale)),  // Apply Offset Y
        static_cast<int>(trans_b.scale * obj_b.w),
        static_cast<int>(trans_b.scale * obj_b.h),
    };

    // Center-based AABB check (Assuming position is Center)
    return (box_a.x - box_a.w / 2 < box_b.x + box_b.w / 2 && box_a.x + box_a.w / 2 > box_b.x - box_b.w / 2 && box_a.y - box_a.h / 2 < box_b.y + box_b.h / 2 &&
            box_a.y + box_a.h / 2 > box_b.y - box_b.h / 2);
}

}  // namespace Engine::Physics