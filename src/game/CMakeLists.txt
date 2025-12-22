file(GLOB_RECURSE GAME_SOURCES "*.cpp")
file(GLOB_RECURSE GAME_HEADERS "*.h")

add_executable(Asteroids
        main.cpp
        ${GAME_SOURCES}
        ${GAME_HEADERS}
)

target_link_libraries(Asteroids PRIVATE Engine)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})