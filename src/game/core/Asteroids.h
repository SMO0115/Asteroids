//
// Created by marcel on 9/28/25.
//

# pragma once

#include "engine/core/CoreModule.h"
#include "game/events/GameEvents.h"

#include <memory>

class IGameState;
class Engine;
class InputManager;
class RenderSystem;

class PlayerControlSystem;
class AISystem;
class HealthSystem;
class LifetimeSystem;
class ScoreSystem;
class UISystem;
class EmitterSystem;


class Asteroids : public IGame {

public:
                                                        Asteroids();
                                                        ~Asteroids()                                override;

    bool                                                init(Engine& engine)                        override;
    void                                                handleInput(InputManager &input_manager)    override;
    bool                                                update(float deltatime, Engine& engine)     override;
    void                                                render(RenderSystem& renderer)              override;


    std::vector<std::unique_ptr<GameObject>>&           getGameObjects()                    { return m_objects;                 }
    std::vector<std::unique_ptr<GameObject>>&           getUIObjects()                      { return m_ui_objects;              }
    std::unique_ptr<GameObject>&                        getGameSession()                    { return m_game_session;            }

    PlayerControlSystem&                                getPlayerControlSystem()  const     { return *m_player_control_system;  }
    AISystem&                                           getAISystem()             const     { return *m_ai_system;              }
    HealthSystem&                                       getHealthSystem()         const     { return *m_health_system;          }
    LifetimeSystem&                                     getLifetimeSystem()       const     { return *m_lifetime_system;        }
    ScoreSystem&                                        getScoreSystem()          const     { return *m_score_system;           }
    UISystem&                                           getUISystem()             const     { return *m_ui_system;              }
    EmitterSystem&                                      getEmitterSystem()        const     { return *m_emitter_system;         }
    GameEventBus&                                       getEventBus()             const     { return *m_event_bus;              }


private:
    std::unique_ptr<IGameState>                         m_state;
    std::vector<std::unique_ptr<GameObject>>            m_objects;
    std::vector<std::unique_ptr<GameObject>>            m_ui_objects;
    std::unique_ptr<GameObject>                         m_game_session;

    std::unique_ptr<PlayerControlSystem>                m_player_control_system;
    std::unique_ptr<AISystem>                           m_ai_system;
    std::unique_ptr<HealthSystem>                       m_health_system;
    std::unique_ptr<LifetimeSystem>                     m_lifetime_system;
    std::unique_ptr<ScoreSystem>                        m_score_system;
    std::unique_ptr<UISystem>                           m_ui_system;

    std::unique_ptr<EmitterSystem>                      m_emitter_system;

    std::unique_ptr<GameEventBus>                       m_event_bus;
};
