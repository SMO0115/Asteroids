file(GLOB_RECURSE ENGINE_SOURCES "*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "*.h")

add_library(Engine STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(Engine PUBLIC ${sdl_mixer_SOURCE_DIR}/include)

target_link_libraries(Engine PUBLIC
        SDL2::SDL2
        SDL2::ttf
        SDL2::image
        SDL2::mixer
        yyjson
)