//
// Created by marcel on 2/11/26.
//

# pragma once

// Precompiled header of the Engine target ONLY (src/engine/CMakeLists.txt): CMake force-includes it
// into every engine .cpp, so these heavy headers are parsed once instead of once per file.
// Don't #include it from other headers: the game and the tests have no PCH, so every file of
// theirs that reached it would parse all of sol2/spdlog/SDL again. Headers include what they use.

#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "glm/glm.hpp"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "sol/sol.hpp"      // <--- MASSIVE (Lua)
#include "spdlog/spdlog.h"  // <--- HEAVY (Logging)
#include "yyjson.h"
