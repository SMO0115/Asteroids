#pragma once

#include <random>

namespace Engine::Core {
class Random {
public:
    static float getFloat(float min, float max) {
        static std::mt19937 mt(std::random_device{}());

        std::uniform_real_distribution<float> dist(min, max);
        return dist(mt);
    }

    static int getInt(int min, int max) {
        static std::mt19937 mt(std::random_device{}());

        std::uniform_int_distribution<int> dist(min, max);
        return dist(mt);
    }
};
}  // namespace Engine::Core