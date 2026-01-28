file(GLOB_RECURSE ENGINE_SOURCES "*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "*.h")

add_library(Engine STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS}
        core/Log.cpp
        core/interfaces/ISystem.h
        scripting/ScriptManager.cpp
        scripting/ScriptManager.h
        scripting/ComponentRegistry.h
        core/Scene.h
        scripting/bindings/ComponentBinding.cpp
        modules/assets/resolveAssets.cpp
        modules/assets/resolveAssets.h
        scripting/StateRegistry.h
)

target_include_directories(Engine PUBLIC ${sol2_SOURCE_DIR}/include)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(Engine PUBLIC ${sdl_mixer_SOURCE_DIR}/include)

target_link_libraries(Engine PUBLIC
        SDL2::SDL2
        SDL2::ttf
        SDL2::image
        SDL2::mixer
        yyjson
        nlohmann_json::nlohmann_json
        spdlog::spdlog
        sol2
        lua_lib
)