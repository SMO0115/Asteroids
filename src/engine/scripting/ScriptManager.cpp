//
// Created by marcel on 1/13/26.
//

#include "ScriptManager.h"

#include "engine/core/Application.h"
#include "engine/core/GameObject.h"
#include "engine/core/Scene.h"
#include "engine/modules/assets/AssetManager.h"

#include "engine/modules/assets/Texture.h"
#include "engine/modules/assets/Animation.h"
#include "engine/modules/assets/Sound.h"
#include "engine/modules/assets/Font.h"

#include "engine/scripting/ComponentRegistry.h"
#include "engine/core/components/TransformComponent.h"



namespace Engine::Scripting {

    ScriptManager::ScriptManager(Engine::Application& engine) {

        m_lua = std::make_unique<sol::state>();
        m_lua->open_libraries(sol::lib::base);

        LUABinding_(engine);
    }

    ScriptManager::~ScriptManager() = default;

    void ScriptManager::runFile(std::string filename) {

        try {
            // Note: CMake copies "assets" to the executable folder
            m_lua->script_file(filename);
        }
        catch (const sol::error& e) {
            std::cerr << "Lua Error: " << e.what() << std::endl;
            return;
        }
    }

    void ScriptManager::LUABinding_(Engine::Application& engine) {

        Assets::AssetManager& asset_manager = engine.getAssetManager();
        Core::Scene* scene_ptr = &engine.getScene();

        m_lua->set_function("loadAnimation", &Assets::AssetManager::loadAnimation, &asset_manager);
        m_lua->set_function("loadTexture",   &Assets::AssetManager::loadTexture  , &asset_manager);
        m_lua->set_function("loadSound",     &Assets::AssetManager::loadSound    , &asset_manager);
        m_lua->set_function("loadFont",      &Assets::AssetManager::loadFont     , &asset_manager);
        

        m_lua->set_function("SpawnEntity", [scene_ptr](const std::string& tag, const std::string& pool_name, sol::table components) {

        // Use the captured pointer
        std::unique_ptr<Core::GameObject> game_object = std::make_unique<Core::GameObject>();

        // 3. Use a safe create method (defined below)
        Pool& pool = scene_ptr->getPool(pool_name);

        for (const auto& comp : components) {
            std::string component_name = comp.first.as<std::string>();
            if (comp.second.is<sol::table>()) {
                sol::table table = comp.second.as<sol::table>();
                ComponentRegistry::Get().addComponent(component_name, *game_object.get(), table);
            }
        }

        pool.push_back(std::move(game_object));
    });
    }
}