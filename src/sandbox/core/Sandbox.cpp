//
// Created by marcel on 9/28/25.
//

#include "Sandbox.h"

#include "engine/core/CoreModule.h"


namespace Game {
Sandbox::Sandbox() = default;

Sandbox::~Sandbox() = default;


bool Sandbox::init(Engine::Core::Context& ctx) {

    ctx.get<Engine::Scripting::ScriptManager>().runFile("assets/scripts/asset_loader.lua");

    return true;
}


void Sandbox::handleInput(Engine::Core::Context& /*ctx*/) {}


bool Sandbox::update(float /*deltatime*/, Engine::Core::Context& /*ctx*/) { return true; }

void Sandbox::render(Engine::Core::Context& /*ctx*/) {}

}  // namespace Game
