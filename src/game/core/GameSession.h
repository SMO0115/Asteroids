//
// Created by smo0115 on 10/24/26.
//

#pragma once

namespace Game::Core {

// Everything about the CURRENT game: the numbers the rules change and the HUD shows.
// A Context service, so every system can reach it; a new game resets it.
struct GameSession {
    static constexpr int START_LIVES      = 3;
    static constexpr int EXTRA_LIFE_EVERY = 10000;  // points

    int score           = 0;
    int lives           = START_LIVES;
    int next_extra_life = EXTRA_LIFE_EVERY;  // the score that gives the next extra life
    int wave            = 0;                 // 0 = no wave yet: the WaveSystem starts wave 1

    // Back to the values above. Assigning a fresh object resets every member, including the ones
    // added later, so this can't forget one (a hand-written list could).
    void reset() { *this = GameSession{}; }
};
}  // namespace Game::Core
