# 1. Find all source files in the sandbox folder
file(GLOB_RECURSE SANDBOX_SOURCES "*.cpp")
file(GLOB_RECURSE SANDBOX_HEADERS "*.h")

# 2. Create the executable
# We assume you have a 'main.cpp' in src/sandbox/
add_executable(Sandbox
        ${SANDBOX_SOURCES}
        ${SANDBOX_HEADERS}
)

# 3. Link the Engine (Fixes 'No such file or directory' for headers)
target_link_libraries(Sandbox PRIVATE Engine)

# 4. Copy Assets (So you can load textures in Sandbox)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})