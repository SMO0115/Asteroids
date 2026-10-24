//
// Created by marcel on 1/26/26.
//

#pragma once
#include <initializer_list>
#include <string>
#include <unordered_map>
#include <utility>

#include "engine/core/Log.h"

namespace Engine::Scripting {

    // Turns names used in Lua/JSON ("IDLE", "PLAYER") into the game's enum values, per enum type
    // ("AnimationState", "CollisionLayer"). The engine only asks; the GAME registers the names at
    // startup. A normal object (a Context service), not a singleton.
    class StateRegistry {
    public:
        StateRegistry()  = default;
        ~StateRegistry() = default;

        StateRegistry(const StateRegistry&)            = delete;
        StateRegistry& operator=(const StateRegistry&) = delete;

        // 1. Register a single value (e.g., "IDLE" -> 0 for "Animation")
        void registerState(const std::string& enumType, const std::string& stateName, int value) {
            m_registry[enumType][stateName] = value;
        }

        // 2. Register a whole batch (Helper)
        void registerBatch(const std::string& enumType, std::initializer_list<std::pair<std::string, int>> pairs) {
            for (const auto& [name, val] : pairs) {
                m_registry[enumType][name] = val;
            }
        }

        // 3. Retrieve (String -> Int). find() instead of operator[], which would insert missing keys
        // (and so can't be used in a const function)
        int getState(const std::string& enumType, const std::string& stateName, int fallback = 0) const {
            if (auto type = m_registry.find(enumType); type != m_registry.end()) {
                if (auto state = type->second.find(stateName); state != type->second.end()) return state->second;
            }
            // Warning: Helpful for debugging typos in Lua
            ENGINE_WARN("[StateRegistry] State '{}' not found in '{}', using {}", stateName, enumType, fallback);
            return fallback;
        }

    private:
        // Map< "EnumTypeName", Map< "StateString", Value > >
        std::unordered_map<std::string, std::unordered_map<std::string, int>> m_registry;
    };
}
