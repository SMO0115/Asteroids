//
// Created by marcel on 1/26/26.
//

#pragma once

#pragma once
#include <string>
#include <unordered_map>
#include <iostream>

namespace Engine::Scripting {

    class StateRegistry {
    public:
        // Singleton Access
        static StateRegistry& Get() {
            static StateRegistry instance;
            return instance;
        }

        // 1. Register a single value (e.g., "IDLE" -> 0 for "Animation")
        bool registerState(const std::string& enumType, const std::string& stateName, int value) {
            m_registry[enumType][stateName] = value;
            return true;
        }

        // 2. Register a whole batch (Helper)
        bool registerBatch(const std::string& enumType, std::initializer_list<std::pair<std::string, int>> pairs) {
            for (const auto& [name, val] : pairs) {
                m_registry[enumType][name] = val;
            }
            return true;
        }

        // 3. Retrieve (String -> Int)
        int getState(const std::string& enumType, const std::string& stateName, int fallback = 0) {
            if (m_registry.contains(enumType) && m_registry[enumType].contains(stateName)) {
                return m_registry[enumType][stateName];
            }
            // Warning: Helpful for debugging typos in Lua
            std::cerr << "[StateRegistry] Warning: State '" << stateName << "' not found in '" << enumType << "'. Using default.\n";
            return fallback;
        }

    private:
        // Map< "EnumTypeName", Map< "StateString", Value > >
        std::unordered_map<std::string, std::unordered_map<std::string, int>> m_registry;

        StateRegistry() = default;
    };
}