//
// Created by marcel on 11/8/25.
//

#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "engine/events/EngineEvents.h"

namespace Engine::Core {
class GameObject;
}
namespace Engine::Assets {
class AssetManager;
}

namespace Engine::Audio {
class SoundSystem {
public:
    SoundSystem();
    ~SoundSystem();

    bool init();

    void playSound(const std::string& sound_id);
    void update(const Assets::AssetManager& asset_manager, Events::EngineEventBus& event_bus,
                const std::vector<std::unique_ptr<Core::GameObject>>& game_objects);

private:
    std::unordered_set<std::string> m_sound_queue;
};
}  // namespace Engine::Audio