//
// Created by marcel on 1/16/26.
//

#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <utility>


namespace Engine::Core { class GameObject; }


namespace Engine::Core {
    template <typename... Events>
    class Scene {
    public:
        template <typename T, typename... TArgs>
        void push(TArgs&&... args) {
            std::get<std::vector<T>>(m_pools).emplace_back(std::forward<TArgs>(args)...);
        }

        template <typename T>
        std::vector<T>& getObjects() {
            return std::get<std::vector<T>>(m_pools);
        }

        void clear() {
            std::apply([](auto&... pool) { (pool.clear(), ...); }, m_pools);
        }

    private:
        std::tuple<std::vector< std::unique_ptr<GameObject> >...> m_pools;
    };
}