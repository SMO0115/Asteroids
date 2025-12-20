//
// Created by marcel on 11/8/25.
//

#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <memory>


#include "../../events/EngineEvents.h"

class GameObject;
class AssetManager;

class SoundSystem {
public:
                                                SoundSystem();
                                                ~SoundSystem();

    bool                                        init();

    void                                        playSound(const std::string& sound_id);
    void                                        update(
                                                    const AssetManager& asset_manager,
                                                    EngineEventBus& event_bus,
                                                    const std::vector<std::unique_ptr<GameObject>>& game_objects
                                                );

private:
    std::unordered_set<std::string>             m_sound_queue;
};
