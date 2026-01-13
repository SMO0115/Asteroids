//
// Created by marcel on 1/13/26.
//

#include "ScriptManager.h"
#include "engine/modules/assets/AssetManager.h"

#include "engine/modules/assets/Texture.h"
#include "engine/modules/assets/Animation.h"
#include "engine/modules/assets/Sound.h"
#include "engine/modules/assets/Font.h"

namespace Engine::Core {

    ScriptManager::ScriptManager(Assets::AssetManager &asset_manager) {

        m_lua.open_libraries(sol::lib::base);
        LUABinding_(asset_manager);
    }

    void ScriptManager::runFile(std::string filename) {

        try {
            // Note: CMake copies "assets" to the executable folder
            m_lua.script_file(filename);
        }
        catch (const sol::error& e) {
            std::cerr << "Lua Error: " << e.what() << std::endl;
            return;
        }
    }

    void ScriptManager::LUABinding_(Assets::AssetManager &asset_manager) {

        m_lua.set_function("loadAnimation", &Assets::AssetManager::loadAnimation, &asset_manager);
        m_lua.set_function("loadTexture",   &Assets::AssetManager::loadTexture  , &asset_manager);
        m_lua.set_function("loadSound",     &Assets::AssetManager::loadSound    , &asset_manager);
        m_lua.set_function("loadFont",      &Assets::AssetManager::loadFont     , &asset_manager);
    }


}