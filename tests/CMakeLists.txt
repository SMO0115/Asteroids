include(FetchContent)

# 1. Download Google Test automatically
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# For Windows: Prevent GTest from overriding parent settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 2. Define the Test Executable
add_executable(AsteroidsTests
        SanityTest.cpp
        # Add other test files here later...
)

# 3. Link dependencies
# We link GTest and your 'Engine' library so we can test engine classes
target_link_libraries(AsteroidsTests
        PRIVATE
        GTest::gtest_main
        Engine  # Ensure your main CMakeLists defines 'Engine' as a library!
)

# 4. Enable CTest (CMake's test runner)
include(GoogleTest)
gtest_discover_tests(AsteroidsTests)