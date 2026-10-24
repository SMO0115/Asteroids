//
// Created by marcel on 1/30/26.
//

#include "game/core/screenWrapper.h"
#include "engine/core/CoreModule.h"
#include "engine/modules/graphics/RenderSystem.h"
#include "engine/modules/graphics/SpriteComponent.h"

namespace Game::Core {

void screenWrapper(Engine::Core::Context& ctx) {

    auto game_objects = ctx.get<Engine::Core::Scene>().getAll();  //.getPool(Game::Core::GamePools::ENEMIES);

    // The screen size in pixels (int), as float for the position math below
    const glm::vec2 screen = glm::vec2(ctx.get<Engine::Graphics::RenderSystem>().getScreenSize());

    for (auto& object : game_objects) {

        if (!object.isActive()) continue;
        if (!object.hasComponent<Engine::Core::TransformComponent>()) continue;
        if (!object.hasComponent<Engine::Graphics::SpriteComponent>()) continue;

        Engine::Core::TransformComponent&  transform = object.getComponent<Engine::Core::TransformComponent>();
        Engine::Graphics::SpriteComponent& sprite    = object.getComponent<Engine::Graphics::SpriteComponent>();


        // Boundaries: the screen plus a sprite margin, so an object is fully off-screen before it wraps
        const float half_w = static_cast<float>(sprite.sourceRect.w) / 2.0f;
        const float half_h = static_cast<float>(sprite.sourceRect.h) / 2.0f;

        float left_limit   = 0.0f - half_w;
        float right_limit  = screen.x + half_w;
        float top_limit    = 0.0f - half_h;
        float bottom_limit = screen.y + half_h;


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
