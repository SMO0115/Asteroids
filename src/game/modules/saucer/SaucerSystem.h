//
// Created by smo0115 on 10/24/26.
//

#pragma once

namespace Engine::Core { class Context; class GameObject; }

namespace Game::Saucer {

    // The flying saucers: now and then one flies in from the left or right edge, zig-zags across the
    // screen once while shooting, and leaves. Only during a wave, and only while the ship is in play.
    // The higher the score, the sooner they come, and the more often it's the small one that aims.
    class SaucerSystem {
    public:
        SaucerSystem()  = default;
        ~SaucerSystem() = default;

        void update(Engine::Core::Context& ctx, float dt);

        // A new game: the first saucer comes after the full delay
        void reset();

        // The game is over: saucers and their shots go with it (their looping sound would go on forever,
        // because the game-over screen doesn't run the SoundSystem)
        static void removeAll(Engine::Core::Context& ctx);

    private:
        static void spawn_(Engine::Core::Context& ctx);
        static void fly_(Engine::Core::Context& ctx, Engine::Core::GameObject& saucer, float dt);
        static void fire_(Engine::Core::Context& ctx, Engine::Core::GameObject& saucer);

        float m_spawn_timer = 0.0f;  // seconds until the next saucer; counts down while none is flying
    };
}  // namespace Game::Saucer
