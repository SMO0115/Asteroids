//
// Created by smo0115 on 4/26/26.
//

#include "engine/core/CoreModule.h"
#include "LifeSystem.h"


namespace Engine::Core {
void LifeSystem::update(Engine::Core::Context& ctx) {

    float deltatime    = static_cast<float>(1.0f / 120.0f);
    auto  game_objects = ctx.get<Engine::Core::Scene>().getAll();

}
}  // namespace Engine::Physics