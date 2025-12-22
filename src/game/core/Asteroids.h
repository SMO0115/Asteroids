//
// Created by marcel on 9/28/25.
//

# pragma once

#include "engine/core/CoreModule.h"
#include "game/events/GameEvents.h"

#include <memory>

namespace Engine            { class Application;         }
namespace Engine::Core      { class GameObject;          }
namespace Engine::Core      { class IGameState;          }
namespace Engine::Core      { class IGame;               }
namespace Engine::Input     { class InputManager;        }
namespace Engine::Graphics  { class RenderSystem;        }



namespace Game::Control     { class PlayerControlSystem; }
namespace Game::Control     { class AISystem;            }
namespace Game::Emitter     { class EmitterSystem;       }
namespace Game::Health      { class HealthSystem;        }
namespace Game::Health      { class LifetimeSystem;      }
namespace Game::UI          { class ScoreSystem;         }
namespace Game::UI          { class UISystem;            }



namespace Game {
    class Asteroids : public Engine::Core::IGame {

    public:
                                                                    Asteroids();
                                                                    ~Asteroids()                                                override;

        bool                                                        init(Engine::Application& engine)                           override;
        void                                                        handleInput(Engine::Input::InputManager &input_manager)     override;
        bool                                                        update(float deltatime, Engine::Application& engine)        override;
        void                                                        render(Engine::Graphics::RenderSystem& renderer)            override;


        std::vector<std::unique_ptr<Engine::Core::GameObject>>&     getGameObjects()                    { return m_objects;                 }
        std::vector<std::unique_ptr<Engine::Core::GameObject>>&     getUIObjects()                      { return m_ui_objects;              }
        std::unique_ptr<Engine::Core::GameObject>&                  getGameSession()                    { return m_game_session;            }

        Control::PlayerControlSystem&                               getPlayerControlSystem()  const     { return *m_player_control_system;  }
        Control::AISystem&                                          getAISystem()             const     { return *m_ai_system;              }
        Health::HealthSystem&                                       getHealthSystem()         const     { return *m_health_system;          }
        Health::LifetimeSystem&                                     getLifetimeSystem()       const     { return *m_lifetime_system;        }
        UI::ScoreSystem&                                            getScoreSystem()          const     { return *m_score_system;           }
        UI::UISystem&                                               getUISystem()             const     { return *m_ui_system;              }
        Emitter::EmitterSystem&                                     getEmitterSystem()        const     { return *m_emitter_system;         }
        Events::GameEventBus&                                       getEventBus()             const     { return *m_event_bus;              }


    private:
        std::unique_ptr<Engine::Core::IGameState>                   m_state;
        std::vector<std::unique_ptr<Engine::Core::GameObject>>      m_objects;
        std::vector<std::unique_ptr<Engine::Core::GameObject>>      m_ui_objects;
        std::unique_ptr<Engine::Core::GameObject>                   m_game_session;

        std::unique_ptr<Control::PlayerControlSystem>               m_player_control_system;
        std::unique_ptr<Control::AISystem>                          m_ai_system;
        std::unique_ptr<Health::HealthSystem>                       m_health_system;
        std::unique_ptr<Health::LifetimeSystem>                     m_lifetime_system;
        std::unique_ptr<UI::ScoreSystem>                            m_score_system;
        std::unique_ptr<UI::UISystem>                               m_ui_system;

        std::unique_ptr<Emitter::EmitterSystem>                     m_emitter_system;
        std::unique_ptr<Events::GameEventBus>                       m_event_bus;
    };
}