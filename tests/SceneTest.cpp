#include <gtest/gtest.h>
#include <memory>

#include "engine/core/GameObject.h"
#include "engine/core/Scene.h"

using namespace Engine::Core;

TEST(SceneTest, DestroyedObjectStaysUntilRemoveDestroyed) {
    Scene scene(2);
    Pool& pool = scene.getPool(0);
    pool.push_back(std::make_unique<GameObject>());
    pool.push_back(std::make_unique<GameObject>());

    GameObject* doomed = pool[0].get();
    doomed->destroy();

    // Still in the pool, so pointers to it (e.g. in a CollisionEvent) stay valid, but switched off
    EXPECT_EQ(pool.size(), 2u);
    EXPECT_FALSE(doomed->isActive());
    EXPECT_TRUE(doomed->isDestroyed());

    scene.removeDestroyed();

    ASSERT_EQ(pool.size(), 1u);
    EXPECT_FALSE(pool[0]->isDestroyed());
}

TEST(SceneTest, RemoveDestroyedChecksEveryPool) {
    Scene scene(3);
    for (std::size_t id = 0; id < 3; ++id) {
        auto object = std::make_unique<GameObject>();
        object->destroy();
        scene.getPool(id).push_back(std::move(object));
    }

    scene.removeDestroyed();

    for (std::size_t id = 0; id < 3; ++id) EXPECT_TRUE(scene.getPool(id).empty());
}

TEST(SceneTest, InactiveObjectsAreNotRemoved) {
    // setActive(false) is for pooling (bullets): the object must survive to be reused
    Scene scene(1);
    scene.getPool(0).push_back(std::make_unique<GameObject>());
    scene.getPool(0)[0]->setActive(false);

    scene.removeDestroyed();

    EXPECT_EQ(scene.getPool(0).size(), 1u);
}
