//
// Created by marcel on 1/13/26.
//

#include "ScriptManager.h"

#include "engine/core/Application.h"
#include "engine/core/GameObject.h"
#include "engine/modules/assets/AssetManager.h"

#include "engine/modules/assets/Texture.h"
#include "engine/modules/assets/Animation.h"
#include "engine/modules/assets/Sound.h"
#include "engine/modules/assets/Font.h"

#include "engine/scripting/ComponentRegistry.h"
#include "engine/core/components/TransformComponent.h"



namespace Engine::Scripting {

    ScriptManager::ScriptManager(Assets::AssetManager &asset_manager) {

        m_lua = std::make_unique<sol::state>();
        m_component_registry = std::make_unique<ComponentRegistry>();

        m_lua->open_libraries(sol::lib::base);
        LUABinding_(asset_manager);
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

        m_lua->set_function("loadAnimation", &Assets::AssetManager::loadAnimation, &asset_manager);
        m_lua->set_function("loadTexture",   &Assets::AssetManager::loadTexture  , &asset_manager);
        m_lua->set_function("loadSound",     &Assets::AssetManager::loadSound    , &asset_manager);
        m_lua->set_function("loadFont",      &Assets::AssetManager::loadFont     , &asset_manager);


        m_component_registry->registerComponent("Transform", [](Engine::Core::GameObject &game_object, const sol::table& t) {

            Core::TransformComponent& transform = game_object.addComponent<Core::TransformComponent>();

            transform.position = glm::vec2(t.get_or("x", 0), t.get_or("y", 0));
            transform.scale    = t.get_or("scale", 1.f);
            transform.rotation = t.get_or("rotation", 0.f);
        });





        m_lua->set_function("SpawnEntity", [&](const std::string& tag, sol::table components) {

            Core::GameObject* game_object = new Core::GameObject();

            for (const auto& comp : components) {

                std::string component_name = comp.first.as<std::string>();

                if (comp.second.is<sol::table>()) {

                    sol::table table = comp.second.as<sol::table>();
                    m_component_registry->addComponent(component_name, *game_object, table);
                }
            }

        });
    }


}