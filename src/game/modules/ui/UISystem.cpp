//
// Created by marcel on 12/13/25.
//

#include "UISystem.h"

#include "engine/modules/rendering/RenderModule.h"
#include "game/core/CoreModule.h"

#include "GameStateComponent.h"


void UISystem::update(float deltaTime,
                      std::vector<std::unique_ptr<GameObject> > &ui_objects,
                      const GameStateComponent &game_state){

    for (const auto &object : ui_objects) {

        if (object->hasComponent<ScoreDisplayTag>()) {

            object->getComponent<UITextComponent>().text = "Score: " + std::to_string( game_state.score );
        }

        if (object->hasComponent<LivesDisplayTag>()) {

            object->getComponent<UITextComponent>().text = "Lives: " + std::to_string( game_state.lives );
        }
    }

}
