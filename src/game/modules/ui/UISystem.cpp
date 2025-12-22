//
// Created by marcel on 12/13/25.
//

#include "UISystem.h"

#include "engine/modules/rendering/RenderModule.h"
#include "game/core/CoreModule.h"

#include "GameStateComponent.h"


namespace Game::UI {
    void UISystem::update(float deltaTime,
                          std::vector<std::unique_ptr<Engine::Core::GameObject> > &ui_objects,
                          const GameStateComponent &game_state){

        for (const auto &object : ui_objects) {

            if (object->hasComponent<Core::ScoreDisplayTag>()) {

                object->getComponent<Engine::Graphics::UITextComponent>().text = "Score: " + std::to_string( game_state.score );
            }

            if (object->hasComponent<Core::LivesDisplayTag>()) {

                object->getComponent<Engine::Graphics::UITextComponent>().text = "Lives: " + std::to_string( game_state.lives );
            }
        }

    }
}