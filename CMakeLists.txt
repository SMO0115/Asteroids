cmake_minimum_required(VERSION 3.20)
project(AsteroidsWorkspace VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# SDL2
FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.3
)

# SDL2_ttf
FetchContent_Declare(
        SDL_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG release-2.22.0
)

# SDL2_image
FetchContent_Declare(
        SDL_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.8.2
)

# SDL2_mixer
FetchContent_Declare(
        SDL_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG release-2.8.0
)

# yyjson
FetchContent_Declare(
        yyjson
        GIT_REPOSITORY https://github.com/ibireme/yyjson.git
        GIT_TAG 0.9.0
)

# Make them available globally
FetchContent_MakeAvailable(SDL2 SDL_ttf SDL_image SDL_mixer yyjson)



add_subdirectory(src/engine)
add_subdirectory(src/game)

enable_testing()
add_subdirectory(tests)