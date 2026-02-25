//
// Created by marcel on 11/8/25.
//

#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "engine/events/EngineEvents.h"

namespace Engine::Core { class GameObject; }
namespace Engine::Assets { class AssetManager; }

namespace Engine::Audio {
class SoundSystem {
public:
    SoundSystem();
    ~SoundSystem();

    bool init();

    void playSound(const Assets::AssetID sound_id);
    void update(Core::Context& ctx);

private:
    std::unordered_set<Assets::AssetID> m_sound_queue;
};
}  // namespace Engine::Audio