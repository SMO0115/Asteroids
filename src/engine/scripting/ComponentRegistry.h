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
        ComponentRegistry() = default;
        ~ComponentRegistry() = default;

        void registerComponent(const std::string& name, ComponentFactory factory) {
            m_factories[name] = factory;
        }

        void addComponent(const std::string& name, Engine::Core::GameObject& obj, const sol::table& data);

    private:
        std::unordered_map<std::string, ComponentFactory> m_factories;
    };
}