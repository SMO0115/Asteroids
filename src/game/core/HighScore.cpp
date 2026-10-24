//
// Created by smo0115 on 10/24/26.
//

#include "HighScore.h"

#include <SDL_error.h>
#include <SDL_filesystem.h>
#include <fstream>

#include "engine/core/Log.h"

namespace Game::Core {

HighScore::HighScore() {

    // A folder the game may write to, the right one for each OS (Linux: ~/.local/share/smo0115/Asteroids/).
    // Not the working directory: that might be read-only, or a different folder each time you start the game.
    if (char* folder = SDL_GetPrefPath("smo0115", "Asteroids")) {
        m_path = std::string(folder) + "highscore.txt";
        SDL_free(folder);  // SDL allocated the string; SDL frees it
    } else {
        LOG_WARN("No folder for the high score ({}): it won't be saved", SDL_GetError());
        return;
    }

    std::ifstream file(m_path);
    if (!(file >> m_best)) m_best = 0;  // no file yet (the first game), or not a number
}


bool HighScore::submit(int score) {

    if (score <= m_best) return false;

    m_best = score;
    save_();
    return true;
}


void HighScore::save_() const {

    if (m_path.empty()) return;

    std::ofstream file(m_path);
    if (!(file << m_best)) LOG_WARN("Could not save the high score to {}", m_path);
}
}  // namespace Game::Core
