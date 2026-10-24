#include <gtest/gtest.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

#include "engine/core/Context.h"
#include "engine/core/GameObject.h"
#include "engine/core/Scene.h"
#include "engine/core/components/TransformComponent.h"
#include "engine/events/EngineEvents.h"
#include "engine/events/EventBus.h"
#include "engine/modules/physics/ColliderComponent.h"
#include "engine/modules/physics/CollisionSystem.h"

using namespace Engine;
using Physics::CollisionSystem;

// ---------------------------------------------------------------------------------
// The math: circle vs circle
// ---------------------------------------------------------------------------------
TEST(CircleCollision, OverlappingCirclesCollide) { EXPECT_TRUE(CollisionSystem::circlesOverlap({100.f, 100.f}, 10.f, {105.f, 105.f}, 10.f)); }

TEST(CircleCollision, SeparatedCirclesDoNotCollide) { EXPECT_FALSE(CollisionSystem::circlesOverlap({100.f, 100.f}, 10.f, {300.f, 300.f}, 10.f)); }

TEST(CircleCollision, TouchingIsNotOverlapping) { EXPECT_FALSE(CollisionSystem::circlesOverlap({0.f, 0.f}, 10.f, {20.f, 0.f}, 10.f)); }

TEST(CircleCollision, BoxCornersNoLongerCount) {
    // 15 px apart on both axes: two 20x20 boxes overlapped here, but the circles are 21.2 px apart
    EXPECT_FALSE(CollisionSystem::circlesOverlap({0.f, 0.f}, 10.f, {15.f, 15.f}, 10.f));
}

TEST(CircleCollision, Benchmark_1MillionChecks) {
    // 1. Setup Data
    const glm::vec2 a = {100.f, 100.f};
    const glm::vec2 b = {105.f, 105.f};

    // 2. Start Timer
    auto start = std::chrono::high_resolution_clock::now();

    // 3. Run a Million Times
    int collisionsFound = 0;
    for (int i = 0; i < 1'000'000; ++i) {
        // We accumulate the result so the compiler doesn't optimize the loop away!
        if (CollisionSystem::circlesOverlap(a, 10.f, b, 10.f)) {
            collisionsFound++;
        }
    }

    // 4. Stop Timer
    auto end      = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // 5. Report
    std::cout << "\n[   PERF   ] 1,000,000 Collision Checks took: " << duration.count() << "ms\n" << std::endl;

    // Only check the result, not the time: a timing assert fails randomly on a slow/busy CI machine
    EXPECT_EQ(collisionsFound, 1'000'000);
}

// ---------------------------------------------------------------------------------
// The whole system: layers, scale, rotated offset -> CollisionEvents
// ---------------------------------------------------------------------------------
class CollisionSystemTest : public ::testing::Test {
protected:
    static constexpr Physics::CollisionMask SHIP = 1 << 0;
    static constexpr Physics::CollisionMask ROCK = 1 << 1;
    static constexpr Physics::CollisionMask SHOT = 1 << 2;

    Core::Context     ctx;
    Core::Scene*      scene     = nullptr;
    Events::EventBus* event_bus = nullptr;
    CollisionSystem   collision_system;

    void SetUp() override {
        scene     = &ctx.registerService<Core::Scene>(1);
        event_bus = &ctx.registerService<Events::EventBus>();
    }

    Core::GameObject& spawn(glm::vec2 position, float radius, Physics::CollisionMask layer, Physics::CollisionMask mask) {
        auto object = std::make_unique<Core::GameObject>();

        object->addComponent<Core::TransformComponent>().position = position;

        auto& collider  = object->addComponent<Physics::ColliderComponent>();
        collider.radius = radius;
        collider.layer  = layer;
        collider.mask   = mask;

        scene->getPool(0).push_back(std::move(object));
        return *scene->getPool(0).back();
    }

    const std::vector<Events::CollisionEvent>& events() const { return event_bus->get<Events::CollisionEvent>(); }
};

TEST_F(CollisionSystemTest, OverlapIsReportedFromBothSides) {
    Core::GameObject& ship = spawn({0.f, 0.f}, 10.f, SHIP, ROCK);
    Core::GameObject& rock = spawn({15.f, 0.f}, 10.f, ROCK, SHIP);

    collision_system.update(ctx);

    ASSERT_EQ(events().size(), 2u);
    EXPECT_EQ(events()[0].from, &ship);
    EXPECT_EQ(events()[0].to, &rock);
    EXPECT_EQ(events()[1].from, &rock);
    EXPECT_EQ(events()[1].to, &ship);
}

TEST_F(CollisionSystemTest, OverlapWithoutMatchingLayersIsIgnored) {
    spawn({0.f, 0.f}, 10.f, SHIP, ROCK);
    spawn({5.f, 0.f}, 10.f, SHOT, ROCK);  // they overlap, but neither cares about the other's layer

    collision_system.update(ctx);

    EXPECT_TRUE(events().empty());
}

TEST_F(CollisionSystemTest, ScaleGrowsTheCircle) {
    Core::GameObject& big = spawn({0.f, 0.f}, 10.f, SHIP, ROCK);

    big.getComponent<Core::TransformComponent>().scale = 3.f;  // radius 30
    spawn({35.f, 0.f}, 10.f, ROCK, SHIP);                      // 35 < 30 + 10

    collision_system.update(ctx);

    EXPECT_EQ(events().size(), 2u);
}

TEST_F(CollisionSystemTest, OffsetTurnsWithTheObject) {
    // The circle sits 30 px in front of the ship. Turned 90 degrees (clockwise on screen, like the
    // sprite), "in front" points DOWN, so it must hit the rock below, not the one to the right.
    Core::GameObject& ship = spawn({0.f, 0.f}, 5.f, SHIP, ROCK);

    ship.getComponent<Physics::ColliderComponent>().offset = {30.f, 0.f};
    ship.getComponent<Core::TransformComponent>().rotation = 90.f;

    Core::GameObject& below = spawn({0.f, 30.f}, 5.f, ROCK, SHIP);
    spawn({30.f, 0.f}, 5.f, ROCK, SHIP);

    collision_system.update(ctx);

    ASSERT_EQ(events().size(), 2u);
    EXPECT_EQ(events()[0].to, &below);
}

TEST_F(CollisionSystemTest, InactiveObjectsDoNotCollide) {
    spawn({0.f, 0.f}, 10.f, SHIP, ROCK);
    spawn({5.f, 0.f}, 10.f, ROCK, SHIP).setActive(false);  // e.g. a bullet waiting in its pool

    collision_system.update(ctx);

    EXPECT_TRUE(events().empty());
}
