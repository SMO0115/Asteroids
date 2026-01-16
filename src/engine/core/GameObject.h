#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <utility>
#include "components/BaseComponent.h"


namespace Engine::Core {
    class GameObject {
    public:
        GameObject() = default;
        virtual ~GameObject() = default;

        template<typename T, typename... TArgs>
        T& addComponent(TArgs&&... args) {

            auto newComponent = std::make_unique<T>(std::forward<TArgs>(args)...);
            T* ptr = static_cast<T*>(newComponent.get());
            m_components[std::type_index(typeid(T))] = std::move(newComponent);

            return *ptr;
        }

        template<typename T>
        void removeComponent() {
            m_components.erase(std::type_index(typeid(T)));
        }

        template<typename T>
        bool hasComponent() const {
            return m_components.contains(std::type_index(typeid(T)));
        }

        template<typename T>
        T& getComponent() {
            auto ptr = m_components.at(std::type_index(typeid(T))).get();
            return *static_cast<T*>(ptr);
        }

        bool isActive() const { return m_isActive; }
        void setActive(bool isActive) { m_isActive = isActive; }

    private:
        bool m_isActive = true;
        std::unordered_map<std::type_index, std::unique_ptr<BaseComponent>> m_components;
    };
}