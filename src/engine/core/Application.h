//
// Created by marcel on 7/31/25.
//
#pragma once

#include <sol/sol.hpp>
#include "glm/glm.hpp"
#include "engine/events/EngineEvents.h"

#include <string>
#include <memory>



namespace Engine::Core     { class IGame;           }
namespace Engine::Core     { class TimeManager;     }
namespace Engine::Core     { class ObjectFactory;   }
namespace Engine::Scripting{ class ScriptManager;   }
namespace Engine::Graphics { class RenderSystem;    }
namespace Engine::Graphics { class AnimationSystem; }
namespace Engine::Audio    { class SoundSystem;     }
namespace Engine::Physics  { class PhysicsSystem;   }
namespace Engine::Physics  { class CollisionSystem; }
namespace Engine::Assets   { class AssetManager;    }
namespace Engine::Input    { class InputManager;    }



namespace Engine {
    class Application {

    public:
                                                            Application();
                                                            ~Application();

        bool                                                init(const std::string& title, std::size_t width, std::size_t height, glm::uint32 target_fps);
        void                                                run(Core::IGame &game);

        void                                                setRunning(const bool running)  { m_is_running = running;       };
        bool                                                isRunning()             const   { return m_is_running;          };


        Scripting::ScriptManager&                           getScriptManager()      const   { return *m_script_manager;     };

        Core::TimeManager&                                  getTimeManager()        const   { return *m_time_manager;       };
        Assets::AssetManager&                               getAssetManager()       const   { return *m_asset_manager;      };
        Input::InputManager&                                getInputManager()       const   { return *m_input_manager;      };

        Graphics::RenderSystem&                             getRenderer()           const   { return *m_renderer;           };
        Graphics::AnimationSystem&                          getAnimationSystem()    const   { return *m_animation_system;   };
        Audio::SoundSystem&                                 getSoundSystem()        const   { return *m_sound_system;       };
        Physics::PhysicsSystem&                             getPhysicsSystem()      const   { return *m_physics;            };
        Physics::CollisionSystem&                           getCollisionSystem()    const   { return *m_collision_system;   };

        Events::EngineEventBus&                             getEventBus()           const   { return *m_event_bus;          };


    private:

        std::unique_ptr<Scripting::ScriptManager>           m_script_manager;

        std::unique_ptr<Core::TimeManager>                  m_time_manager;
        std::unique_ptr<Assets::AssetManager>               m_asset_manager;
        std::unique_ptr<Input::InputManager>                m_input_manager;

        std::unique_ptr<Graphics::RenderSystem>             m_renderer;
        std::unique_ptr<Graphics::AnimationSystem>          m_animation_system;
        std::unique_ptr<Audio::SoundSystem>                 m_sound_system;
        std::unique_ptr<Physics::PhysicsSystem>             m_physics;
        std::unique_ptr<Physics::CollisionSystem>           m_collision_system;

        std::unique_ptr<Events::EngineEventBus>             m_event_bus;

        bool                                                m_is_running;
        bool                                                m_quit_requested;
    };
}