cmake_minimum_required(VERSION 3.20)

# IMPORTANT: Added 'C' to LANGUAGES for Lua compilation
project(AsteroidsWorkspace VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# --- EXISTING LIBS ---
FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.3
)

FetchContent_Declare(
        SDL_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG release-2.22.0
)

FetchContent_Declare(
        SDL_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.8.2
)

FetchContent_Declare(
        SDL_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG release-2.8.0
)

FetchContent_Declare(
        yyjson
        GIT_REPOSITORY https://github.com/ibireme/yyjson.git
        GIT_TAG 0.9.0
)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.12.0
)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.12.0
)

# --- NEW: LUA 5.4 SETUP ---
# Lua does not have a CMake file, so we fetch it and build the library manually
FetchContent_Declare(
        lua_source
        URL https://www.lua.org/ftp/lua-5.4.6.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_GetProperties(lua_source)
if(NOT lua_source_POPULATED)
    FetchContent_Populate(lua_source)

    # 1. Gather all C files
    file(GLOB LUA_SOURCES "${lua_source_SOURCE_DIR}/src/*.c")

    # 2. Remove the standalone interpreter (lua.c) and compiler (luac.c)
    # We only want the library code, not their main() functions
    list(FILTER LUA_SOURCES EXCLUDE REGEX "lua\\.c")
    list(FILTER LUA_SOURCES EXCLUDE REGEX "luac\\.c")

    # 3. Create the library target
    add_library(lua_lib STATIC ${LUA_SOURCES})

    # 4. Allow including "lua.h"
    target_include_directories(lua_lib PUBLIC "${lua_source_SOURCE_DIR}/src")

    # 5. Linux specific: Lua needs 'dl' library for loading modules
    if(UNIX AND NOT APPLE)
        target_link_libraries(lua_lib PUBLIC dl)
    endif()
endif()

FetchContent_Declare(
        sol2
        GIT_REPOSITORY https://github.com/ThePhD/sol2.git
        GIT_TAG        v3.3.0
)

FetchContent_MakeAvailable(SDL2 SDL_ttf SDL_image SDL_mixer yyjson json spdlog sol2)


add_subdirectory(src/engine)
add_subdirectory(src/game)

enable_testing()
add_subdirectory(tests)