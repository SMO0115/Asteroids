//
// Created by marcel on 1/25/26.
//

#pragma once

namespace Engine::Core { class Context; class GameObject; }

namespace Engine::Assets {

    class AssetManager;

    // Turn asset names stored in components ("asteroid1") into AssetIDs
    void resolveAssets(Engine::Core::GameObject& obj, AssetManager& asset_manager);  // one object
    void resolveAssets(Engine::Core::Context& ctx);                                  // every object in the scene
}
