//
// Created by marcel on 1/13/26.
//

#pragma once
#include <sol/sol.hpp>
#include <string>


namespace Engine            { class Application;       }
namespace Engine::Scripting { class ComponentRegistry; }
namespace Engine::Assets    { class AssetManager;      }

namespace Engine::Scripting {
    class ScriptManager {

    public:
        ScriptManager(Engine::Application& engine);
        ~ScriptManager();

        void runFile(std::string filename);
        // void runConsole();

    private:
        void LUABinding_(Engine::Application& engine);

    private:

        std::unique_ptr<sol::state>               m_lua;
    };
}