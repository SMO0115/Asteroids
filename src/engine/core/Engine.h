//
// Created by marcel on 7/31/25.
//
#pragma once

#include "glm/glm.hpp"
#include "engine/events/EngineEvents.h"

#include <string>
#include <memory>

class IGame;

class RenderSystem;
class SoundSystem;
class AnimationSystem;
class PhysicsSystem;
class CollisionSystem;
class AssetManager;
class InputManager;
class TimeManager;




class Engine {

public:
                                        Engine();
                                        ~Engine();

    bool                                init(const std::string& title, std::size_t width, std::size_t height, glm::uint32 target_fps);
    void                                run(IGame &game);

    void                                setRunning(const bool running)  { m_is_running = running;       };
    bool                                isRunning()             const   { return m_is_running;          };


    RenderSystem&                       getRenderer()           const   { return *m_renderer;           };
    SoundSystem&                        getSoundSystem()        const   { return *m_sound_system;       };
    AnimationSystem&                    getAnimationSystem()    const   { return *m_animation_system;   };
    PhysicsSystem&                      getPhysicsSystem()      const   { return *m_physics;            };
    CollisionSystem&                    getCollisionSystem()    const   { return *m_collision_system;   };

    AssetManager&                       getAssetManager()       const   { return *m_asset_manager;      };
    InputManager&                       getInputManager()       const   { return *m_input_manager;      };
    TimeManager&                        getTimeManager()        const   { return *m_time_manager;       };

    EngineEventBus&                     getEventBus()           const   { return *m_event_bus;          };


private:
    std::unique_ptr<AssetManager>       m_asset_manager;
    std::unique_ptr<InputManager>       m_input_manager;
    std::unique_ptr<TimeManager>        m_time_manager;

    std::unique_ptr<RenderSystem>       m_renderer;
    std::unique_ptr<SoundSystem>        m_sound_system;
    std::unique_ptr<AnimationSystem>    m_animation_system;
    std::unique_ptr<PhysicsSystem>      m_physics;
    std::unique_ptr<CollisionSystem>    m_collision_system;

    std::unique_ptr<EngineEventBus>     m_event_bus;

    bool                                m_is_running;
    bool                                m_quit_requested;
};
