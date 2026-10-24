#pragma once
#include <sol/sol.hpp>
#include <functional>
#include <unordered_map>
#include <string>

#include "engine/core/Log.h"

// Include your GameObject definition
namespace Engine::Core { class GameObject; }

namespace Engine::Scripting {

    using ComponentFactory = std::function<void(Engine::Core::GameObject&, const sol::table&)>;

    // Maps the component names used in Lua ("Transform", "Asteroid", ...) to functions that add that
    // component to a GameObject.
    //
    // A normal object (a Context service), not a singleton: the engine and the game fill it with
    // explicit calls at startup, in an order you can see, and a test can simply make its own.
    class ComponentRegistry {
    public:
        ComponentRegistry()  = default;
        ~ComponentRegistry() = default;

        // Delete copy constructors to prevent accidental copies
        ComponentRegistry(const ComponentRegistry&)            = delete;
        ComponentRegistry& operator=(const ComponentRegistry&) = delete;

        // --- PUBLIC API ---

        void registerComponent(const std::string& name, ComponentFactory factory) {
            m_factories[name] = std::move(factory);
        }

        void addComponent(const std::string& name, Engine::Core::GameObject& obj, const sol::table& data) const {
            auto it = m_factories.find(name);
            if (it != m_factories.end()) {
                it->second(obj, data);
            } else {
                ENGINE_WARN("[Script] Unknown component '{}'", name);
            }
        }

    private:
        std::unordered_map<std::string, ComponentFactory> m_factories;
    };
}
