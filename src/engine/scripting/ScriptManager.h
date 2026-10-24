//
// Created by marcel on 1/13/26.
//

#pragma once
#include <sol/sol.hpp>
#include <string>

#include "engine/core/Log.h"


namespace Engine::Core      { class Context;           }

namespace Engine::Scripting {
    class ScriptManager {

    public:
        ScriptManager(Engine::Core::Context& ctx);
        ~ScriptManager();

        void runFile(std::string filename);

        // Call a global Lua function from C++: call("SpawnAsteroid", x, y, vx, vy, size).
        // Like runFile(), a Lua error is printed instead of crashing the game.
        template <typename... Args>
        void call(const std::string& function, Args&&... args) {

            // get<T> converts explicitly; '= (*m_lua)[function]' left the compiler two conversions to pick from
            sol::protected_function lua_function = m_lua->get<sol::protected_function>(function);
            if (!lua_function.valid()) {
                ENGINE_ERROR("[Script] No Lua function called '{}'", function);
                return;
            }

            sol::protected_function_result result = lua_function(std::forward<Args>(args)...);
            if (!result.valid()) {
                sol::error error = result;
                ENGINE_ERROR("[Script] Lua error in {}(): {}", function, error.what());
            }
        }

    private:
        void LUABinding_(Engine::Core::Context& ctx);

    private:

        std::unique_ptr<sol::state>               m_lua;
    };
}
