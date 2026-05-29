//
// Created by marcel on 9/28/25.
//

#pragma once

#include "engine/core/CoreModule.h"
#include "game/events/GameEvents.h"

#include <memory>

namespace Engine            { class Application;            }
namespace Engine::Core      { class GameObject;             }
namespace Engine::Core      { class IGameState;             }
namespace Engine::Core      { class IGame;                  }
namespace Engine::Input     { class InputManager;           }
namespace Engine::Graphics  { class RenderSystem;           }


namespace Game {
class Asteroids : public Engine::Core::IGame {
public:
    Asteroids();
    ~Asteroids() override;

    bool init(Engine::Core::Context& ctx) override;
    void handleInput(Engine::Core::Context& ctx) override;
    bool update(float deltatime, Engine::Core::Context& ctx) override;
    void render(Engine::Core::Context& ctx) override;

    // std::vector<std::unique_ptr<Engine::Core::GameObject>>& getGameObjects() { return m_objects;      }
    // std::vector<std::unique_ptr<Engine::Core::GameObject>>& getUIObjects()   { return m_ui_objects;   }
    // std::unique_ptr<Engine::Core::GameObject>&              getGameSession() { return m_game_session; }


private:
    std::unique_ptr<Engine::Core::IGameState>              m_state;
    // std::vector<std::unique_ptr<Engine::Core::GameObject>> m_objects;
    // std::vector<std::unique_ptr<Engine::Core::GameObject>> m_ui_objects;
    // std::unique_ptr<Engine::Core::GameObject>              m_game_session;
};
}  // namespace Game
    