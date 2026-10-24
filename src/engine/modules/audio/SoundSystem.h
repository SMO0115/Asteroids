//
// Created by marcel on 11/8/25.
//

#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "engine/events/EngineEvents.h"
#include "engine/utils/Types.h"

namespace Engine::Core { class GameObject; class Context; }
namespace Engine::Assets { class AssetManager; }

namespace Engine::Audio { struct SoundComponent; }

namespace Engine::Audio {
class SoundSystem {
public:
    SoundSystem();
    ~SoundSystem();

    bool init();

    void playSound(const Assets::AssetID sound_id);
    void update(Core::Context& ctx);

    // For the game states: a pause menu pauses everything (a looping thrust would go on forever
    // otherwise, because update() doesn't run while paused), and a new game stops everything.
    // Call stopAll() together with Scene::clear(): a SoundComponent that survives it would still
    // hold a channel number.
    void pauseAll();
    void resumeAll();
    void stopAll();

private:
    void stopChannel_(SoundComponent& sound_comp);

    std::unordered_set<Assets::AssetID> m_sound_queue;
};
}  // namespace Engine::Audio