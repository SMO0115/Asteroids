#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <typeindex>

namespace Engine::Core { class GameObject; }

using Pool = std::vector<std::unique_ptr<Engine::Core::GameObject>>;

namespace Engine::Core {
    class Scene {
    public:
        Scene() = default;
        ~Scene() = default;


        Pool& registerPool(const std::string &pool_name, size_t initial_capacity = 1024) {

            auto& pool = m_pools[pool_name];
            if (initial_capacity > 0) pool.reserve(initial_capacity);

            return pool;
        }

        // 2. Unregister
        void unregisterPool(const std::string &pool_name) {
            m_pools.erase(pool_name);
        }

        // 3. Check existence
        bool hasPool(const std::string &pool_name) const {
            return m_pools.contains(pool_name);
        }

        // 4. Get: Returns the generic vector
        Pool& getPool(const std::string &pool_name) {
            return m_pools.at(pool_name);
        }

        std::unordered_map<std::string, Pool>& getAllPools() { return m_pools; }

    private:
        std::unordered_map<std::string, Pool> m_pools;
    };
}