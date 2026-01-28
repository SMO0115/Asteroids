#pragma once
#include <sol/sol.hpp>
#include <functional>
#include <unordered_map>
#include <string>
#include <iostream>

// Include your GameObject definition
namespace Engine::Core { class GameObject; }

namespace Engine::Scripting {

    using ComponentFactory = std::function<void(Engine::Core::GameObject&, const sol::table&)>;

    class ComponentRegistry {
    public:
        // --- THE SINGLETON ACCESSOR ---
        // This static method is the ONLY way to get the registry.
        static ComponentRegistry& Get() {
            static ComponentRegistry instance; // Guaranteed to be created once
            return instance;
        }

        // Delete copy constructors to prevent accidental copies
        ComponentRegistry(const ComponentRegistry&) = delete;
        void operator=(const ComponentRegistry&) = delete;

        // --- PUBLIC API ---

        // Note: Returns bool so we can use it in static initializers!
        bool registerComponent(const std::string& name, ComponentFactory factory) {
            m_factories[name] = factory;
            return true;
        }

        void addComponent(const std::string& name, Engine::Core::GameObject& obj, const sol::table& data){
            if (m_factories.find(name) != m_factories.end()) {
                m_factories[name](obj, data);
            } else {
                std::cerr << "[Script] Warning: Unknown component '" << name << "'" << std::endl;
            }
        }

    private:
        // Private Constructor: Ensures no one can say "new ComponentRegistry()"
        ComponentRegistry() = default;
        ~ComponentRegistry() = default;

        std::unordered_map<std::string, ComponentFactory> m_factories;
    };
}