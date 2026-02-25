//
// Created by marcel on 1/30/26.
//

#include "game/core/screenWrapper.h"
#include "engine/core/CoreModule.h"
#include "game/modules/player/PlayerSystem.h"

namespace Game::Core {

void screenWrapper(Engine::Core::Context& ctx) {

    auto game_objects = ctx.get<Engine::Core::Scene>().getAll();  //.getPool(Game::Core::GamePools::ENEMIES);

    for (auto& object : game_objects) {

        if (!object.hasComponent<Engine::Core::TransformComponent>()) continue;
        if (!object.hasComponent<Engine::Graphics::SpriteComponent>()) continue;

        Engine::Core::TransformComponent&  transform = object.getComponent<Engine::Core::TransformComponent>();
        Engine::Graphics::SpriteComponent& sprite    = object.getComponent<Engine::Graphics::SpriteComponent>();


        // Define your boundaries (adjust to your window size + sprite margin)
        float left_limit   = 0.0f - sprite.sourceRect.w / 2;
        float right_limit  = 800.0f + sprite.sourceRect.w / 2;
        float top_limit    = 0.0f - sprite.sourceRect.h / 2;
        float bottom_limit = 800.0f + sprite.sourceRect.h / 2;


        // 1. Check X Axis
        if (transform.position.x < left_limit) {
            transform.position.x = right_limit;
        } else if (transform.position.x > right_limit) {
            transform.position.x = left_limit;
        }

        // 2. Check Y Axis
        if (transform.position.y < top_limit) {
            transform.position.y = bottom_limit;
        } else if (transform.position.y > bottom_limit) {
            transform.position.y = top_limit;
        }
    }
}
}  // namespace Game::Core
