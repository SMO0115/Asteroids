//
// Created by marcel on 1/13/26.
//

#include "ScriptManager.h"

#include "engine/core/Application.h"
#include "engine/core/Context.h"
#include "engine/core/GameObject.h"
#include "engine/core/Scene.h"
#include "engine/modules/assets/AssetManager.h"

#include "engine/modules/assets/Animation.h"
#include "engine/modules/assets/Font.h"
#include "engine/modules/assets/Sound.h"
#include "engine/modules/assets/Texture.h"
#include "engine/modules/assets/resolveAssets.h"
#include "engine/modules/graphics/RenderSystem.h"

#include "engine/core/components/TransformComponent.h"
#include "engine/scripting/ComponentRegistry.h"


namespace Engine::Scripting {

ScriptManager::ScriptManager(Engine::Core::Context& ctx) {

    m_lua = std::make_unique<sol::state>();
    m_lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);

    LUABinding_(ctx);
}

ScriptManager::~ScriptManager() = default;

void ScriptManager::runFile(std::string filename) {

    try {
        // Note: CMake copies "assets" to the executable folder
        m_lua->script_file(filename);
    } catch (const sol::error& e) {
        ENGINE_ERROR("[Script] Lua error: {}", e.what());
        return;
    }
}

void ScriptManager::LUABinding_(Engine::Core::Context& ctx) {

    Assets::AssetManager& asset_manager = ctx.get<Engine::Assets::AssetManager>();
    Core::Scene*          scene_ptr     = &ctx.get<Engine::Core::Scene>();

    m_lua->set_function("loadAnimation", &Assets::AssetManager::loadAnimation, &asset_manager);
    m_lua->set_function("loadTexture", &Assets::AssetManager::loadTexture, &asset_manager);
    m_lua->set_function("loadSound", &Assets::AssetManager::loadSound, &asset_manager);
    m_lua->set_function("loadFont", &Assets::AssetManager::loadFont, &asset_manager);

    // Lua: local width, height = GetScreenSize()   (a tuple becomes two return values in Lua)
    const Graphics::RenderSystem* renderer_ptr = &ctx.get<Graphics::RenderSystem>();
    m_lua->set_function("GetScreenSize", [renderer_ptr]() {
        const glm::ivec2 size = renderer_ptr->getScreenSize();
        return std::make_tuple(size.x, size.y);
    });


    Assets::AssetManager*    assets_ptr   = &asset_manager;
    const ComponentRegistry* registry_ptr = &ctx.get<ComponentRegistry>();  // filled by the engine AND the game

    // The first argument ("Asteroid", "Bullet", ...) only labels the call in Lua for now
    auto spawn_entity = [scene_ptr, assets_ptr, registry_ptr](const std::string& /*tag*/, const std::string& pool_name, const bool is_active,
                                                              sol::table components) {
        std::optional<std::size_t> pool_id = scene_ptr->getPoolID(pool_name);
        if (!pool_id) {
            ENGINE_ERROR("[Script] SpawnEntity: unknown pool '{}', entity not created", pool_name);
            return;
        }

        // Use the captured pointer
        std::unique_ptr<Core::GameObject> game_object = std::make_unique<Core::GameObject>();

        game_object->setActive(is_active);

        for (const auto& comp : components) {
            std::string component_name = comp.first.as<std::string>();
            if (comp.second.is<sol::table>()) {
                sol::table table = comp.second.as<sol::table>();
                registry_ptr->addComponent(component_name, *game_object, table);
            }
        }

        // Turn asset names into IDs right away, so objects spawned during the game (e.g. asteroid
        // fragments) can be drawn on the same tick
        Assets::resolveAssets(*game_object, *assets_ptr);

        scene_ptr->getPool(*pool_id).push_back(std::move(game_object));
    };
    m_lua->set_function("SpawnEntity", spawn_entity);
}
}  // namespace Engine::Scripting