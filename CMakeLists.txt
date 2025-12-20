# Minimum version of CMake required
cmake_minimum_required(VERSION 3.20)

# Define the project name, C++ version, etc.
project(Asteroids VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Fetch Dependencies from GitHub ---
include(FetchContent)

# 1. SDL2
FetchContent_Declare(
        SDL
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.3
)

# 2. SDL2_ttf
FetchContent_Declare(
        SDL_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG release-2.22.0
)

# 3. SDL2_image
FetchContent_Declare(
        SDL_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.8.2
)

# 4. SDL2_mixer
FetchContent_Declare(
        SDL_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG release-2.8.0
)

# 5. yyjson
FetchContent_Declare(
        yyjson
        GIT_REPOSITORY https://github.com/ibireme/yyjson.git
        GIT_TAG 0.9.0
)

# Download and make them available
FetchContent_MakeAvailable(SDL SDL_ttf SDL_image SDL_mixer yyjson)

# --- Automatic Source Discovery ---
# Instead of listing files manually, we scan the directory tree.
# This finds ANY .cpp or .h file inside src/engine and src/game,
# even inside sub-folders like 'modules/physics' or 'core'.

file(GLOB_RECURSE ENGINE_SOURCES "src/engine/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "src/engine/*.h")

file(GLOB_RECURSE GAME_SOURCES "src/game/*.cpp")
file(GLOB_RECURSE GAME_HEADERS "src/game/*.h")

# --- Executable ---
add_executable(Asteroids
        src/main.cpp
        ${ENGINE_SOURCES}
        ${ENGINE_HEADERS}
        ${GAME_SOURCES}
        ${GAME_HEADERS}
        src/engine/modules/rendering/RenderModule.h
        src/engine/modules/physics/PhysicsModule.h
        src/engine/modules/input/InputModule.h
        src/engine/modules/audio/AudioModule.h
        src/engine/modules/assets/AssetModule.h
        src/engine/core/CoreModule.h
        src/game/core/CoreModule.h
        src/game/modules/control/ControlModul.h
        src/game/modules/emitter/EmiterModule.h
        src/game/modules/health/HealthModule.h
        src/game/modules/ui/UIModule.h
)

# --- Include Directories ---
# This line is critical!
# It allows you to write #include "engine/core/Engine.h"
# instead of relative paths like ../../../engine/core/Engine.h
target_include_directories(Asteroids PRIVATE src)

# Also include SDL_mixer headers if needed directly
target_include_directories(Asteroids PRIVATE ${sdl_mixer_SOURCE_DIR}/include)

# --- Linking ---
target_link_libraries(Asteroids PRIVATE SDL2::SDL2 SDL2::ttf SDL2::image SDL2::mixer yyjson)

# --- Copy Assets (Optional but Recommended) ---
# If you have an 'assets' folder next to src, this copies it to the build folder
# so the game can find images/sounds when you run it.
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})