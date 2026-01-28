//
// Created by marcel on 12/20/25.
//

#pragma once

#include <map>
#include <unordered_map>
#include <vector>
#include <array>
#include <string>
#include <memory>
#include <iostream>


#include "glm/glm.hpp"
#include "components/BaseComponent.h"
#include "Application.h"
#include "GameObject.h"
#include "Scene.h"
#include "interfaces/IGame.h"
#include "interfaces/IGameState.h"
#include "TimeManager.h"
#include "../scripting/ScriptManager.h"
#include "Log.h"

#include "components/TransformComponent.h"


#include "engine/scripting/bindings/ComponentBinding.cpp"
