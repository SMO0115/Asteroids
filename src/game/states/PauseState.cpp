//
// Created by marcel on 9/29/25.
//

#include "../../game/states/PauseState.h"
#include "RunState.h"

#include "engine/modules/input/InputModule.h"
#include "engine/modules/physics/PhysicsModule.h"
#include "engine/modules/rendering/RenderModule.h"
#include "engine/modules/assets/AssetModule.h"


PauseState::PauseState(Asteroids& game) : m_game(game) {


}


PauseState::~PauseState() = default;


std::unique_ptr<IGameState> PauseState::handleInput(InputManager &input_manager) {

    if ( input_manager.wasKeyPressed(Key::ESCAPE) ) return std::make_unique<RunState>(m_game);

    return nullptr;
}

std::unique_ptr<IGameState> PauseState::update(float deltatime, Engine& engine) {



    std::unique_ptr<GameObject> Pause = std::make_unique<GameObject>();

    Font* font = engine.getAssetManager().getFont("pixel",72);
    Pause->addComponent<UITextComponent>(
        "PAUSE",
        font,
        Color{255, 255, 255}, 290, 340
        );
    m_pause.push_back(std::move(Pause));

    return nullptr;
}

void PauseState::render(RenderSystem &renderer) {

    renderer.renderWorld(m_game.getGameObjects());
    renderer.renderUI(m_game.getUIObjects());

    renderer.drawRect({0, 0, 800, 800}, Color{0, 0, 0, 192});
    renderer.renderUI(m_pause);
}
