//
// Created by smo0115 on 10/24/26.
//

#pragma once

#include <glm/vec2.hpp>

namespace Engine::Core { class Context; }

namespace Game::Asteroid {

    // Starts the next wave when the last asteroid is gone: after a short pause, 4, 6, 8, 10 and
    // then 11 large asteroids (like the arcade) come in from the screen edges.
    // During a wave it plays the arcade's "heartbeat", which gets faster the longer the wave lasts
    // (and pauses while there is no ship in play).
    class WaveSystem {
    public:
        WaveSystem()  = default;
        ~WaveSystem() = default;

        void update(Engine::Core::Context& ctx, float dt);

        // A new game: the next update() starts wave 1 right away, without the pause
        void reset();

        // 'count' large asteroids on the screen edges, flying in random directions, none of them
        // closer than a safe distance to 'avoid' (the ship)
        static void spawnAsteroids(Engine::Core::Context& ctx, int count, glm::vec2 avoid);

    private:
        void beat_(Engine::Core::Context& ctx, float dt);
        void restartBeat_();

        float m_next_wave_timer = 0.0f;   // seconds until the next wave; counts down while the sky is empty

        float m_beat_interval   = 0.0f;   // seconds between two beats; shrinks during a wave
        float m_beat_timer      = 0.0f;   // seconds until the next beat
        bool  m_beat_second     = false;  // which of the two notes comes next
    };
}  // namespace Game::Asteroid
