//
// Created by marcel on 1/13/26.
//

#pragma once
#include <sol/sol.hpp>
#include <string>


namespace Engine::Core      { class Context;           }

namespace Engine::Scripting {
    class ScriptManager {

    public:
        ScriptManager(Engine::Core::Context& ctx);
        ~ScriptManager();

        void runFile(std::string filename);
        // void runConsole();

    private:
        void LUABinding_(Engine::Core::Context& ctx);

    private:

        std::unique_ptr<sol::state>               m_lua;
    };
}