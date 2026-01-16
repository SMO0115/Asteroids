//
// Created by marcel on 1/13/26.
//

#pragma once
#include <sol/sol.hpp>
#include <string>


namespace Engine::Scripting { class ComponentRegistry; }
namespace Engine::Assets    { class AssetManager;      }

namespace Engine::Scripting {
    class ScriptManager {

    public:
        ScriptManager(Assets::AssetManager &asset_manager);
        ~ScriptManager();

        void runFile(std::string filename);
        // void runConsole();

    private:
        void LUABinding_(Assets::AssetManager &asset_manager);

    private:

        std::unique_ptr<sol::state>               m_lua;
        std::unique_ptr<ComponentRegistry>        m_component_registry;
    };
}