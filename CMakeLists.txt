cmake_minimum_required(VERSION 3.25)  # 3.25+: SYSTEM option for FetchContent

# IMPORTANT: Added 'C' to LANGUAGES for Lua compilation
project(AsteroidsWorkspace VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ASTEROIDS_SANITIZE "Build our own targets with AddressSanitizer + UndefinedBehaviorSanitizer" OFF)

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

# Warnings (and optionally sanitizers) for OUR targets only. Third-party code is fetched
# with SYSTEM below, so its headers don't flood the output with warnings we can't fix.
function(asteroids_target_options target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
    endif()

    if(ASTEROIDS_SANITIZE)
        target_compile_options(${target} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=address,undefined)
    endif()
endfunction()

include(FetchContent)

# --- EXISTING LIBS ---
FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.3
        SYSTEM
)

FetchContent_Declare(
        SDL_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG release-2.22.0
        SYSTEM
)

FetchContent_Declare(
        SDL_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.8.2
        SYSTEM
)

FetchContent_Declare(
        SDL_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG release-2.8.0
        SYSTEM
)

FetchContent_Declare(
        yyjson
        GIT_REPOSITORY https://github.com/ibireme/yyjson.git
        GIT_TAG 0.9.0
        SYSTEM
)

# GLM (vector math). It used to be found only because Arch installs it in /usr/include: the CI
# runner has no GLM, so the build failed there. Fetched like everything else, every machine
# builds with the same version.
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        1.0.1
        SYSTEM
)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.12.0
        SYSTEM
)

# --- LUA 5.4 SETUP ---
# Lua does not have a CMake file: MakeAvailable only downloads it, and we build the library manually
FetchContent_Declare(
        lua_source
        URL https://www.lua.org/ftp/lua-5.4.6.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_Declare(
        sol2
        GIT_REPOSITORY https://github.com/ThePhD/sol2.git
        GIT_TAG        v3.3.0
        SYSTEM
)

# We only play WAV files. These SDL_mixer codecs need extra system libraries
# (a clean CI machine doesn't have them), so don't build them.
set(SDL2MIXER_OPUS    OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_MOD     OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_MIDI    OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_WAVPACK OFF CACHE BOOL "" FORCE)

# GLM is header-only for us: no compiled library, no tests
set(GLM_BUILD_LIBRARY OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_TESTS   OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(lua_source)

# 1. Gather all C files
file(GLOB LUA_SOURCES "${lua_source_SOURCE_DIR}/src/*.c")

# 2. Remove the standalone interpreter (lua.c) and compiler (luac.c)
# We only want the library code, not their main() functions
list(FILTER LUA_SOURCES EXCLUDE REGEX "lua\\.c")
list(FILTER LUA_SOURCES EXCLUDE REGEX "luac\\.c")

# 3. Create the library target
add_library(lua_lib STATIC ${LUA_SOURCES})

# 4. Allow including "lua.h"
target_include_directories(lua_lib SYSTEM PUBLIC "${lua_source_SOURCE_DIR}/src")

# 5. Linux specific: Lua needs 'dl' library for loading modules
if(UNIX AND NOT APPLE)
    target_link_libraries(lua_lib PUBLIC dl)
endif()

FetchContent_MakeAvailable(SDL2 SDL_ttf SDL_image SDL_mixer yyjson glm spdlog sol2)

# SDL 2.30's PipeWire backend passes a wrong pointer type, and GCC 14+ made that an error.
# Turn it back into a warning for SDL only, instead of for every C file in the project.
if(NOT MSVC)
    foreach(sdl_target SDL2 SDL2-static)
        if(TARGET ${sdl_target})
            target_compile_options(${sdl_target} PRIVATE $<$<COMPILE_LANGUAGE:C>:-Wno-error=incompatible-pointer-types>)
        endif()
    endforeach()
endif()

# sol2 3.3.0 has a broken template that is never used, and GCC 14+ reports it anyway.
# INTERFACE: every target that links sol2 (and so includes its headers) gets the flag.
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-Wtemplate-body HAS_WTEMPLATE_BODY)
if(HAS_WTEMPLATE_BODY)
    target_compile_options(sol2 INTERFACE -Wno-template-body)
endif()


add_subdirectory(src/engine)
add_subdirectory(src/game)
add_subdirectory(src/sandbox)

enable_testing()
add_subdirectory(tests)
