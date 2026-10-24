cmake_minimum_required(VERSION 3.20)
project(Asteroids)

# 1. Gather Sources
# CONFIGURE_DEPENDS: the build re-checks the globs, so new files are picked up without a manual CMake reload
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB_RECURSE GAME_HEADERS CONFIGURE_DEPENDS "*.h")

# 2. Define the Executable (main.cpp is part of GAME_SOURCES)
add_executable(Asteroids
        ${GAME_SOURCES}
        ${GAME_HEADERS}
)
asteroids_target_options(Asteroids)

# 3. Link your Engine library
target_link_libraries(Asteroids PRIVATE Engine)

# 4. THE FIX: Sync assets every time you build
# This runs after the executable is linked and copies the folder
# to the exact location where the .exe / binary lives.
add_custom_command(TARGET Asteroids POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:Asteroids>/assets"
        COMMENT "Syncing assets to build directory..."
)