//
// Created by marcel on 11/8/25.
//

#include <SDL_mixer.h>

#include "engine/core/CoreModule.h"
#include "engine/core/Log.h"

#include "SoundSystem.h"
#include "engine/events/EngineEvents.h"


#include "SoundComponent.h"
#include "engine/modules/assets/AssetModule.h"


namespace Engine::Audio {
SoundSystem::SoundSystem() = default;
SoundSystem::~SoundSystem() {

    Mix_CloseAudio();
    Mix_Quit();
}


bool SoundSystem::init() {
    SDL_Init(SDL_INIT_AUDIO);

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        ENGINE_FATAL("[Sound] Mix_OpenAudio: {}", Mix_GetError());
        return false;
    }

    Mix_AllocateChannels(256);
    return true;
}


void SoundSystem::playSound(const Assets::AssetID sound_id) { m_sound_queue.insert(sound_id); }


void SoundSystem::update(Core::Context& ctx) {


    auto                  game_objects  = ctx.get<Engine::Core::Scene>().getAll();
    Events::EventBus&     event_bus     = ctx.get<Events::EventBus>();
    Assets::AssetManager& asset_manager = ctx.get<Engine::Assets::AssetManager>();

    for (auto& object : game_objects) {

        if (!object.hasComponent<SoundComponent>()) continue;
        auto& sound_comp = object.getComponent<SoundComponent>();

        // A switched-off object (e.g. the ship after a hit) is silent. Forget its state, so the
        // state's sound starts again when the object comes back.
        if (!object.isActive()) {
            stopChannel_(sound_comp);
            sound_comp.previous_state = -1;
            continue;
        }

        if (sound_comp.current_state == sound_comp.previous_state) continue;
        sound_comp.previous_state = sound_comp.current_state;

        stopChannel_(sound_comp);  // the old state's (looping) sound

        auto it = sound_comp.sounds_ids.find(sound_comp.current_state);
        if (it == sound_comp.sounds_ids.end()) continue;  // this state has no sound (e.g. IDLE)

        Assets::Sound* sound = asset_manager.getSound(it->second);
        if (sound == nullptr || sound->m_sdl_chunk == nullptr) continue;

        // Played right here, not queued: a looping sound needs its channel, to stop it later.
        // Only a loop keeps the channel number. A one-shot sound frees its channel when it ends and
        // SDL_mixer reuses it, so halting that number later could cut off some other sound.
        if (sound_comp.is_looping) {
            sound_comp.channel = Mix_PlayChannel(-1, sound->m_sdl_chunk.get(), -1);  // -1 = loop forever
        } else {
            Mix_PlayChannel(-1, sound->m_sdl_chunk.get(), 0);
        }
    }


    for (const Events::PlaySoundEvent& event : event_bus.get<Events::PlaySoundEvent>()) {
        playSound(asset_manager.getSoundID(event.soundId));
    }

    for (const auto& sound_id : m_sound_queue) {
        Assets::Sound* sound = asset_manager.getSound(sound_id);

        if (sound != nullptr && sound->m_sdl_chunk != nullptr) {
            Mix_PlayChannel(-1, sound->m_sdl_chunk.get(), 0);  // 0 = play once
        }
    }
    m_sound_queue.clear();
}


void SoundSystem::pauseAll() { Mix_Pause(-1); }  // -1 = every channel

void SoundSystem::resumeAll() { Mix_Resume(-1); }

void SoundSystem::stopAll() { Mix_HaltChannel(-1); }


void SoundSystem::stopChannel_(SoundComponent& sound_comp) {

    if (sound_comp.channel == -1) return;

    Mix_HaltChannel(sound_comp.channel);
    sound_comp.channel = -1;
}
}  // namespace Engine::Audio