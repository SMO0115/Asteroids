//
// Created by marcel on 9/29/25.
//

#include "../../game/states/LevelCompletedState.h"

#include "RunState.h"

#include "engine/modules/input/InputModule.h"
#include "engine/modules/physics/PhysicsModule.h"
#include "engine/modules/rendering/RenderModule.h"
#include "engine/modules/assets/AssetModule.h"




LevelCompletedState::LevelCompletedState(Asteroids& game) : m_game(game) {


}


LevelCompletedState::~LevelCompletedState() = default;


std::unique_ptr<IGameState> LevelCompletedState::handleInput(InputManager &input_manager) {

    if ( input_manager.wasKeyPressed(Key::ESCAPE) ) return std::make_unique<RunState>(m_game);

    return nullptr;
}

std::unique_ptr<IGameState> LevelCompletedState::update(float deltatime, Engine& engine) {



    std::unique_ptr<GameObject> Pause = std::make_unique<GameObject>();

    Font* font = engine.getAssetManager().getFont("pixel",72);
    Pause->addComponent<UITextComponent>(
        "!!! VICTORY !!!",
        font,
        Color{255, 255, 255}, 190, 340
        );
    m_text.push_back(std::move(Pause));


    return nullptr;
}

void LevelCompletedState::render(RenderSystem &renderer) {

    renderer.renderWorld(m_game.getGameObjects());
    renderer.renderUI(m_game.getUIObjects());

    renderer.drawRect({0, 0, 800, 800}, Color{0, 0, 0, 192});
    renderer.renderUI(m_text);
}
