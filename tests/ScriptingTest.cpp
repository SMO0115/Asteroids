#include <gtest/gtest.h>
#include <sol/sol.hpp>

#include "engine/core/GameObject.h"
#include "engine/core/Scene.h"
#include "engine/core/components/TransformComponent.h"
#include "engine/modules/physics/ColliderComponent.h"
#include "engine/scripting/ComponentRegistry.h"
#include "engine/scripting/StateRegistry.h"
#include "engine/scripting/bindings/ComponentBinding.h"

using namespace Engine;

// No singletons: every test builds its own registries, so tests can't leak state into each other
class ScriptingTest : public ::testing::Test {
protected:
    sol::state                   lua;
    Scripting::StateRegistry     states;
    Scripting::ComponentRegistry components;

    void SetUp() override { Scripting::registerEngineComponents(components, states); }

    sol::table table(const std::string& lua_code) { return lua.script("return " + lua_code); }
};

TEST_F(ScriptingTest, TransformKeepsFractionalPositions) {
    Core::GameObject obj;
    components.addComponent("Transform", obj, table("{ x = 12.5, y = 3.25 }"));

    const auto& transform = obj.getComponent<Core::TransformComponent>();
    EXPECT_FLOAT_EQ(transform.position.x, 12.5f);
    EXPECT_FLOAT_EQ(transform.position.y, 3.25f);
}

TEST_F(ScriptingTest, ColliderBitsComeFromTheNamesTheGameRegistered) {
    // The engine knows no layer names at all: here the "game" (this test) invents its own
    states.registerBatch("CollisionLayer", {{"SHIP", 1 << 0}, {"ROCK", 1 << 1}, {"SHOT", 1 << 2}});

    Core::GameObject obj;
    components.addComponent("Collider", obj, table("{ layer = 'SHIP', mask = { 'ROCK', 'SHOT' }, radius = 12.5, offset = { x = 3, y = -1 } }"));

    const auto& collider = obj.getComponent<Physics::ColliderComponent>();
    EXPECT_EQ(collider.layer, 0b001u);
    EXPECT_EQ(collider.mask, 0b110u);
    EXPECT_FLOAT_EQ(collider.radius, 12.5f);
    EXPECT_FLOAT_EQ(collider.offset.x, 3.f);
    EXPECT_FLOAT_EQ(collider.offset.y, -1.f);
}

TEST_F(ScriptingTest, GameCanAddItsOwnComponents) {
    struct Shield : Core::BaseComponent {
        int strength = 0;
    };
    components.registerComponent("Shield", [](Core::GameObject& obj, const sol::table& t) { obj.addComponent<Shield>().strength = t.get_or("strength", 0); });

    Core::GameObject obj;
    components.addComponent("Shield", obj, table("{ strength = 7 }"));

    EXPECT_EQ(obj.getComponent<Shield>().strength, 7);
}

TEST_F(ScriptingTest, UnknownComponentIsIgnored) {
    Core::GameObject obj;
    components.addComponent("NoSuchComponent", obj, table("{}"));  // prints a warning, must not crash

    EXPECT_FALSE(obj.hasComponent<Core::TransformComponent>());
}

TEST(ScenePoolNames, LookupByName) {
    Core::Scene scene(4);
    scene.registerName(2, "Enemies");

    EXPECT_EQ(scene.getPoolID("Enemies"), std::optional<std::size_t>{2});
    EXPECT_EQ(scene.getName(2), "Enemies");
    EXPECT_FALSE(scene.getPoolID("Enemys").has_value());  // a typo is reported, not mapped to pool 0
}
