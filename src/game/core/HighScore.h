//
// Created by smo0115 on 10/24/26.
//

#pragma once

#include <string>

namespace Game::Core {

    // The best score ever, kept in a file so it survives closing the game.
    // A Context service; unlike the GameSession, a new game doesn't reset it.
    class HighScore {
    public:
        HighScore();  // reads the file (no file yet = 0)
        ~HighScore() = default;

        int best() const { return m_best; }

        // A finished game's score. Returns true (and saves) if it's a new record.
        bool submit(int score);

    private:
        void save_() const;

        std::string m_path;  // empty if SDL couldn't give us a folder: then nothing is saved
        int         m_best = 0;
    };
}  // namespace Game::Core
