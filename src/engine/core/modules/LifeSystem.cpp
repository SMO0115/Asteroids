//
// Created by smo0115 on 4/26/26.
//

#include "LifeSystem.h"
#include "engine/core/CoreModule.h"
#include "engine/core/components/LifeComponent.h"


namespace Engine::Core {
void LifeSystem::update(Engine::Core::Context& ctx, float dt) {

    auto game_objects = ctx.get<Engine::Core::Scene>().getAll();

    for (auto& object : game_objects) {

        if (!object.isActive()) continue;
        if (!object.hasComponent<LifeComponent>()) continue;

        LifeComponent& life = object.getComponent<LifeComponent>();
        if (life.life_time <= 0.0f) continue;  // 0 = no time limit

        life.life_time -= dt;

        if (life.life_time > 0.0f) continue;

        if (life.destroy_on_expire) {
            object.destroy();  // erased at the end of the tick
        } else {
            object.setActive(false);  // pooled: reused, not re-created
        }
    }
}
}  // namespace Engine::Core
