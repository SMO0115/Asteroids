#include <gtest/gtest.h>
#include <chrono>
#include <iostream>
#include "engine/core/components/TransformComponent.h"
#include "engine/modules/physics/CollisionSystem.h"
#include "engine/utils/Types.h"

using namespace Engine;

class CollisionTest : public ::testing::Test {
protected:
    Physics::CollisionSystem collisionSystem;

    // --- THE WRAPPER METHOD ---
    // Since 'CollisionTest' is the friend, IT can call the private function.
    // The tests below (which inherit from this class) will call this public helper.
    bool checkCollisionHelper(const Core::TransformComponent& ta, const Core::TransformComponent& tb, const Core::Rect& ra, const Core::Rect& rb) {
        return collisionSystem.checkCollisionAABB_(ta, tb, ra, rb);
    }
};

TEST_F(CollisionTest, DetectsCollisionUsingTransforms) {
    Core::TransformComponent transA;
    transA.position  = {100.0f, 100.0f};
    Core::Rect rectA = {0, 0, 10, 10};

    Core::TransformComponent transB;
    transB.position  = {105.0f, 105.0f};
    Core::Rect rectB = {0, 0, 10, 10};

    // Use the helper, not the private function directly
    EXPECT_TRUE(checkCollisionHelper(transA, transB, rectA, rectB));
}

TEST_F(CollisionTest, IgnoresSeparatedTransforms) {
    Core::TransformComponent transA;
    transA.position  = {100.0f, 100.0f};
    Core::Rect rectA = {0, 0, 10, 10};

    Core::TransformComponent transB;
    transB.position  = {300.0f, 300.0f};
    Core::Rect rectB = {0, 0, 10, 10};

    EXPECT_FALSE(checkCollisionHelper(transA, transB, rectA, rectB));
}

TEST_F(CollisionTest, Benchmark_1MillionChecks) {
    // 1. Setup Data
    Core::TransformComponent tA;
    tA.position = {100.f, 100.f};
    tA.rotation = 0.0f;
    tA.scale    = 1.0f;

    Core::TransformComponent tB;
    tB.position = {105.f, 105.f};
    tB.rotation = 0.0f;
    tB.scale    = 1.0f;

    Core::Rect rA = {0, 0, 10, 10};
    Core::Rect rB = {0, 0, 10, 10};

    // 2. Start Timer
    auto start = std::chrono::high_resolution_clock::now();

    // 3. Run a Million Times
    int collisionsFound = 0;
    for (int i = 0; i < 1'000'000; ++i) {
        // We accumulate the result so the compiler doesn't optimize the loop away!
        if (checkCollisionHelper(tA, tB, rA, rB)) {
            collisionsFound++;
        }
    }

    // 4. Stop Timer
    auto end      = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // 5. Report
    std::cout << "\n[   PERF   ] 1,000,000 Collision Checks took: " << duration.count() << "ms\n" << std::endl;

    // Optional: Fail if it's too slow (e.g., slower than 100ms)
    EXPECT_LT(duration.count(), 100);
}