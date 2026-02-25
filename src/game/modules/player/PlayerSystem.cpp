//
// Created by marcel on 2/10/26.
//

#include "PlayerSystem.h"
#include "engine/core/CoreModule.h"
#include "engine/modules/input/InputManager.h"
#include <glm/gtx/rotate_vector.hpp>

// #include "sandbox/core/GameTypes.h"

glm::mat2 getRotationMatrix2D(float angle) {

    angle = glm::radians(angle);
    float c = glm::cos(angle);
    float s = glm::sin(angle);
    return glm::mat2(c, s, -s, c); // Note: GLM is column-major!
}

namespace Game::Player {
void PlayerSystem::update(Engine::Core::Context& ctx) {
    Engine::Input::InputManager& input = ctx.get<Engine::Input::InputManager>();
    auto& player  = *ctx.get<Engine::Core::Scene>().getPool(Game::Core::GamePools::PLAYER)[0].get();


    Engine::Graphics::AnimationComponent& anim = player.getComponent<Engine::Graphics::AnimationComponent>();
    Engine::Physics::PhysicsComponent& physics = player.getComponent<Engine::Physics::PhysicsComponent>();
    Engine::Core::TransformComponent& transform = player.getComponent<Engine::Core::TransformComponent>();
    LOG_INFO(transform.rotation);

    anim.current_state = Game::Core::AnimationState::IDLE;
    anim.current_duration = 1.0f;

    if (input.isKeyHeld(Engine::Input::Key::UP)) {

        physics.velocity += getRotationMatrix2D(transform.rotation) *  glm::vec2(2.0, 0.0);
        anim.current_state = Game::Core::AnimationState::RUN;
    }

    if (input.isKeyHeld(Engine::Input::Key::DOWN)) {

        physics.velocity += getRotationMatrix2D(transform.rotation) *  glm::vec2(-2.0, 0.0);
    }

    if (input.isKeyHeld(Engine::Input::Key::LEFT)) {

        physics.angularVelocity -= 1.5f;
    }

    if (input.isKeyHeld(Engine::Input::Key::RIGHT)) {

        physics.angularVelocity += 1.5f;
    }


}
}  // namespace Game::Player
