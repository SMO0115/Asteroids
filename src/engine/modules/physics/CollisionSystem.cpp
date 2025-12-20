//
// Created by marcel on 11/22/25.
//

#include <iostream>
#include "engine/core/CoreModule.h"


#include "ColliderComponent.h"
#include "CollisionSystem.h"

#include "engine/events/EventBus.h"




std::string layerToString(CollisionLayer layer) {

    if (layer == CollisionLayer::PLAYER)            return "Player";
    if (layer == CollisionLayer::INVADER)           return "Enemy";
    if (layer == CollisionLayer::PLAYER_BULLET)     return "PlayerBullet";
    if (layer == CollisionLayer::INVADER_BULLET)    return "EnemyBullet";
    if (layer == CollisionLayer::WALL)              return "Wall";

    return "Unknown";
}

void CollisionSystem::update(EngineEventBus& event_bus, const std::vector<std::unique_ptr<GameObject> >& game_objects) {

    for (size_t i = 0; i < game_objects.size(); ++i) {
        for (size_t j = i + 1; j < game_objects.size(); ++j) {

            GameObject* obj_a = game_objects[i].get();
            GameObject* obj_b = game_objects[j].get();

            if (!obj_a->isActive() || !obj_b->isActive()) continue;

            if (!obj_a->hasComponent<ColliderComponent>() ||
                !obj_b->hasComponent<ColliderComponent>()) continue;

            const auto& col_a = obj_a->getComponent<ColliderComponent>();
            const auto& col_b = obj_b->getComponent<ColliderComponent>();

            bool a_hits_b = static_cast<int>(col_a.mask & col_b.layer) != 0;
            bool b_hits_a = static_cast<int>(col_b.mask & col_a.layer) != 0;

            if (!a_hits_b && !b_hits_a) continue;

            const auto& trans_a = obj_a->getComponent<TransformComponent>();
            const auto& trans_b = obj_b->getComponent<TransformComponent>();

            if (checkCollisionAABB_(trans_a, trans_b, col_a.bounds, col_b.bounds)) {

                if (a_hits_b) {
                    std::cout << "[Collision] " << layerToString(col_a.layer)
                              << " (ID:" << i << ") hit "
                              << layerToString(col_b.layer) << " (ID:" << j << ")" << std::endl;
                }
                if (b_hits_a) {
                    std::cout << "[Collision] " << layerToString(col_b.layer)
                              << " (ID:" << j << ") hit "
                              << layerToString(col_a.layer) << " (ID:" << i << ")" << std::endl;
                }

                if (a_hits_b) event_bus.publish<CollisionEvent>(obj_a, obj_b);
                if (b_hits_a) event_bus.publish<CollisionEvent>(obj_b, obj_a);
            }
        }
    }

}



bool CollisionSystem::checkCollisionAABB_(  const TransformComponent& trans_a,
                                            const TransformComponent& trans_b,
                                            const Rect&                 obj_a,
                                            const Rect&                 obj_b)  {

    Rect box_a = {
        static_cast<int>( trans_a.position.x ),
        static_cast<int>( trans_a.position.y ),
        static_cast<int>( trans_a.scale * obj_a.w),
        static_cast<int>( trans_a.scale * obj_a.h),
    };

    Rect box_b = {
        static_cast<int>( trans_b.position.x ),
        static_cast<int>( trans_b.position.y ),
        static_cast<int>( trans_b.scale * obj_b.w),
        static_cast<int>( trans_b.scale * obj_b.h),
    };


    if (box_a.x - box_a.w / 2 < box_b.x + box_b.w / 2 &&
        box_a.x + box_a.w / 2 > box_b.x - box_b.w / 2 &&
        box_a.y - box_a.h / 2 < box_b.y + box_b.h / 2 &&
        box_a.y + box_a.h / 2 > box_b.y - box_b.h / 2 ) return true;

    return false;
}