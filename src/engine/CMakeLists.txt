# CONFIGURE_DEPENDS: the build re-checks the globs, so new files are picked up without a manual CMake reload
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS "*.h")

# The engine must not know about any game: that is what keeps it reusable for the next one.
# Fail the configure step if an engine file includes a game header.
foreach(engine_file IN LISTS ENGINE_SOURCES ENGINE_HEADERS)
    file(STRINGS "${engine_file}" game_includes REGEX "#include [<\"](game|sandbox)/")
    if(game_includes)
        message(FATAL_ERROR "${engine_file} includes game code: ${game_includes}")
    endif()
endforeach()

add_library(Engine STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})
asteroids_target_options(Engine)

target_precompile_headers(Engine PRIVATE core/pch.h)

target_include_directories(Engine SYSTEM PUBLIC ${sol2_SOURCE_DIR}/include)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(Engine SYSTEM PUBLIC ${sdl_mixer_SOURCE_DIR}/include)

target_link_libraries(Engine PUBLIC
        SDL2::SDL2
        SDL2_ttf::SDL2_ttf      # the old names (SDL2::ttf, ...) are deprecated
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer
        yyjson                  # JSON (the animation files); nlohmann_json was linked too, but never used
        glm::glm                # PUBLIC like the rest: the game and the tests include GLM through engine headers
        spdlog::spdlog
        sol2
        lua_lib
)
