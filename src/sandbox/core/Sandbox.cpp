//
// Created by marcel on 9/28/25.
//

#include "glm/glm.hpp"


#include "Sandbox.h"

#include "engine/core/CoreModule.h"

#include "engine/modules/assets/AssetModule.h"
#include "engine/modules/audio/AudioModule.h"
#include "engine/modules/graphics/RenderModule.h"
#include "engine/modules/physics/PhysicsModule.h"

#include "components/Tags.h"

#include "game/events/GameEvents.h"

#include "game/states/RunState.h"


struct Entities {};

namespace Game {
Sandbox::Sandbox() { m_event_bus = std::make_unique<Events::GameEventBus>(); }

Sandbox::~Sandbox() {}


bool Sandbox::init(Engine::Core::Context& ctx) {

    engine.getScriptManager().runFile("assets/scripts/asset_loader.lua");


    Engine::Core::Scene& scene = engine.getScene();


    return true;
}


void Sandbox::handleInput(Engine::Core::Context& ctx) {}


bool Sandbox::update(float deltatime, Engine::Core::Context& ctx) {


    Engine::Core::Scene& scene = engine.getScene();

    m_event_bus->clear();

    return true;
}

void Sandbox::render(Engine::Core::Context& ctx) {}

}  // namespace Game
