//
// Created by marcel on 11/8/25.
//

#include <iostream>

#include "SDL.h"
#include "SDL_mixer.h"

#include "engine/core/CoreModule.h"

#include "SoundSystem.h"
#include "../../events/EventBus.h"


#include "SoundComponent.h"
#include "engine/modules/assets/AssetModule.h"


SoundSystem::SoundSystem() = default;
SoundSystem::~SoundSystem() {

    Mix_CloseAudio();
    Mix_Quit();
}


bool SoundSystem::init() {
    SDL_Init( SDL_INIT_AUDIO);

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    Mix_AllocateChannels(256);
    return true;
}


void SoundSystem::playSound(const std::string &sound_id) {

    m_sound_queue.insert(sound_id);
}


void SoundSystem::update(const AssetManager& asset_manager, EngineEventBus& event_bus, const std::vector<std::unique_ptr<GameObject>>& game_objects) {


    for (const auto &object : game_objects) {

        if (!object->hasComponent<SoundComponent>()) continue;
        auto& sound_comp = object->getComponent<SoundComponent>();


        if (sound_comp.current_state != sound_comp.previous_state) {

            if (sound_comp.channel != -1) {
                Mix_HaltChannel(sound_comp.channel);
                sound_comp.channel = -1;
            }

            auto it = sound_comp.sounds.find(sound_comp.current_state);
            if (it != sound_comp.sounds.end()) playSound(it->second);

            sound_comp.previous_state = sound_comp.current_state;
        }
    }



    for (const PlaySoundEvent& event : event_bus.getEvents<PlaySoundEvent>()) playSound(event.soundId);

    for (const auto &sound_id : m_sound_queue) {
        Sound* sound = asset_manager.getSound(sound_id);

        if (sound != nullptr && sound->m_sdl_chunk != nullptr) {
            Mix_PlayChannel(-1, sound->m_sdl_chunk, 0); // 0 = play once
        }
    }
    m_sound_queue.clear();
}