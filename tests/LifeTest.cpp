#include <gtest/gtest.h>
#include <memory>

#include "engine/core/Context.h"
#include "engine/core/GameObject.h"
#include "engine/core/Scene.h"
#include "engine/core/components/LifeComponent.h"
#include "engine/core/modules/LifeSystem.h"

using namespace Engine::Core;

class LifeTest : public ::testing::Test {
protected:
    LifeTest() { ctx.registerService<Scene>(1); }

    // An active object in pool 0 that lives 'seconds'
    GameObject& spawn(float seconds, bool destroy_on_expire) {
        auto  object           = std::make_unique<GameObject>();
        auto& life             = object->addComponent<LifeComponent>();
        life.life_time         = seconds;
        life.destroy_on_expire = destroy_on_expire;
        GameObject& ref        = *object;
        ctx.get<Scene>().getPool(0).push_back(std::move(object));
        return ref;
    }

    Context    ctx;
    LifeSystem life_system;
};

TEST_F(LifeTest, PooledObjectIsOnlySwitchedOff) {
    GameObject& bullet = spawn(1.0f, false);

    life_system.update(ctx, 0.5f);
    EXPECT_TRUE(bullet.isActive());

    life_system.update(ctx, 0.6f);
    EXPECT_FALSE(bullet.isActive());
    EXPECT_FALSE(bullet.isDestroyed());  // stays in the pool, to be reused
}

TEST_F(LifeTest, OneShotObjectIsDestroyed) {
    GameObject& debris = spawn(0.2f, true);

    life_system.update(ctx, 0.3f);
    EXPECT_TRUE(debris.isDestroyed());
}

TEST_F(LifeTest, ZeroLifeTimeMeansNoLimit) {
    GameObject& ship = spawn(0.0f, true);

    life_system.update(ctx, 100.0f);
    EXPECT_TRUE(ship.isActive());
}
