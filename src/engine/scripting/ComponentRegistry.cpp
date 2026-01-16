//
// Created by marcel on 1/16/26.
//

#include "ComponentRegistry.h"
#include "engine/core/GameObject.h"

namespace Engine::Scripting {

    void ComponentRegistry::addComponent(const std::string& name, Engine::Core::GameObject& obj, const sol::table& data) {
        if (m_factories.find(name) != m_factories.end()) {
            m_factories[name](obj, data);
        } else {
            std::cerr << "[Script] Warning: Unknown component '" << name << "'" << std::endl;
        }
    }

}