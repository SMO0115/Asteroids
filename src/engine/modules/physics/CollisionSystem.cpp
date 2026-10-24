//
// Created by marcel on 11/22/25.
//

#include "CollisionSystem.h"

#include <glm/geometric.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "ColliderComponent.h"
#include "engine/core/CoreModule.h"
#include "engine/events/EventBus.h"

namespace Engine::Physics {

struct ColliderCache {
    Core::GameObject*  obj;
    ColliderComponent* col;
    glm::vec2          center;  // circle center in the world
    float              radius;  // already multiplied by the scale
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
            ColliderComponent&              col   = obj.getComponent<ColliderComponent>();
            const Core::TransformComponent& trans = obj.getComponent<Core::TransformComponent>();

            // The world-space circle, once per object instead of once per pair. The offset turns with
            // the object (the same rotation the sprite gets), so it stays e.g. on the ship's nose.
            const glm::vec2 offset = glm::rotate(col.offset * trans.scale, glm::radians(trans.rotation));
            active_colliders.push_back({&obj, &col, trans.position + offset, col.radius * trans.scale});
        }
    }

    size_t count = active_colliders.size();
    for (size_t i = 0; i < count; ++i) {

        ColliderCache& cache_a = active_colliders[i];

        for (size_t j = i + 1; j < count; ++j) {

            ColliderCache& cache_b = active_colliders[j];

            // Optimization: Bitwise check is extremely fast, do it first
            bool a_hits_b = (cache_a.col->mask & cache_b.col->layer) != 0;
            bool b_hits_a = (cache_b.col->mask & cache_a.col->layer) != 0;

            if (!a_hits_b && !b_hits_a) continue;

            if (circlesOverlap(cache_a.center, cache_a.radius, cache_b.center, cache_b.radius)) {

                if (a_hits_b) event_bus.publish<Events::CollisionEvent>(cache_a.obj, cache_b.obj);
                if (b_hits_a) event_bus.publish<Events::CollisionEvent>(cache_b.obj, cache_a.obj);
            }
        }
    }
}

bool CollisionSystem::circlesOverlap(glm::vec2 center_a, float radius_a, glm::vec2 center_b, float radius_b) {

    // Compare the SQUARED distance with the squared reach: same answer, but no square root needed
    const glm::vec2 distance = center_b - center_a;
    const float     reach    = radius_a + radius_b;
    return glm::dot(distance, distance) < reach * reach;
}

}  // namespace Engine::Physics
