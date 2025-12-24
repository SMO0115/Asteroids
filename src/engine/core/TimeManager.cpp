#include "TimeManager.h"
#include <SDL2/SDL.h>


namespace Engine::Core {
void TimeManager::start() {

    m_frequency = SDL_GetPerformanceFrequency();
    m_lastTime  = SDL_GetPerformanceCounter();
}

double TimeManager::tick() {

    uint64_t now  = SDL_GetPerformanceCounter();
    uint64_t diff = now - m_lastTime;

    m_deltaTime = static_cast<double>(diff) / static_cast<double>(m_frequency);

    m_lastTime = now;
    return m_deltaTime;
}

double TimeManager::getNow() const {
    uint64_t now = SDL_GetPerformanceCounter();
    return static_cast<double>(now) / static_cast<double>(m_frequency);
}
}  // namespace Engine::Core