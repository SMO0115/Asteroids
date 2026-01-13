//
// Created by marcel on 1/13/26.
//

#pragma once
#include <sol/sol.hpp>
#include <string>


namespace Engine::Assets   { class AssetManager;    }

namespace Engine::Core {
    class ScriptManager {

    public:
        ScriptManager(Assets::AssetManager &asset_manager);
        ~ScriptManager() = default;

        void runFile(std::string filename);
        // void runConsole();

    private:
        void LUABinding_(Assets::AssetManager &asset_manager);

    private:

        sol::state m_lua;
    };
}