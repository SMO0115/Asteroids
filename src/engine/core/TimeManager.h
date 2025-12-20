//
// Created by marcel on 9/28/25.
//

#pragma once
#include <glm/glm.hpp>

class TimeManager {
public:
    TimeManager() : m_frequency(0), m_lastTime(0), m_deltaTime(0.0) {};

    void                            start();
    double                          tick();
    double                          getDeltaTime()  const   { return m_deltaTime; }
    double                          getNow()        const;

private:
    glm::uint64_t                   m_frequency;
    glm::uint64_t                   m_lastTime;
    double                          m_deltaTime;
};