#pragma once
#include <vector>
#include <memory>
#include <string>
#include <cassert>
#include <type_traits>

namespace Engine::Core {

    class GameObject; // Forward declaration

    using Pool = std::vector<std::unique_ptr<GameObject>>;

    // ==========================================
    // 1. THE FLATTENED ITERATOR
    // ==========================================
    class MultiPoolIterator {
    public:
        // Iterator traits
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = GameObject;
        using pointer           = GameObject*;
        using reference         = GameObject&;

        // Stores pointers to the pools we want to iterate
        std::vector<Pool*> m_targetPools;
        size_t m_currentPoolIdx = 0;
        Pool::iterator m_objectIter;

        MultiPoolIterator(std::vector<Pool*> pools, bool isEnd)
            : m_targetPools(std::move(pools))
        {
            if (isEnd) {
                m_currentPoolIdx = m_targetPools.size();
            } else {
                m_currentPoolIdx = 0;
                findNextValid();
            }
        }

        reference operator*() const { return *(*m_objectIter); }
        pointer operator->() { return m_objectIter->get(); }

        MultiPoolIterator& operator++() {
            ++m_objectIter;
            // If we hit the end of the current pool, move to next
            if (m_objectIter == m_targetPools[m_currentPoolIdx]->end()) {
                m_currentPoolIdx++;
                findNextValid();
            }
            return *this;
        }

        bool operator!=(const MultiPoolIterator& other) const {
            if (m_currentPoolIdx != other.m_currentPoolIdx) return true;
            if (m_currentPoolIdx >= m_targetPools.size()) return false;
            return m_objectIter != other.m_objectIter;
        }

    private:
        void findNextValid() {
            while (m_currentPoolIdx < m_targetPools.size()) {
                if (!m_targetPools[m_currentPoolIdx]->empty()) {
                    m_objectIter = m_targetPools[m_currentPoolIdx]->begin();
                    return;
                }
                m_currentPoolIdx++;
            }
        }
    };

    // ==========================================
    // 2. THE VIEW HELPER
    // ==========================================
    struct MultiPoolView {
        std::vector<Pool*> pools;

        auto begin() { return MultiPoolIterator(pools, false); }
        auto end()   { return MultiPoolIterator(pools, true); }
    };

    // ==========================================
    // 3. THE SCENE CLASS
    // ==========================================
    class Scene {
    public:
        // Scene() = default;
        ~Scene() = default;

        Scene(size_t poolCount = 128) {
            m_pools.resize(poolCount);
            m_poolNames.resize(poolCount, "Unnamed");
            for (auto& p : m_pools) p = std::make_unique<Pool>();
        }

        // 1. Single Pool Access
        Pool& getPool(size_t id) {
            assert(id < m_pools.size() && "Pool ID out of bounds!");
            return *m_pools[id];
        }

        // 2. Subset View: scene.get<GamePools::Enemies, GamePools::Bullets>()
        template<size_t... PoolIDs>
        MultiPoolView get() {
            MultiPoolView view;
            view.pools.reserve(sizeof...(PoolIDs));
            (view.pools.push_back(m_pools[PoolIDs].get()), ...);
            return view;
        }

        // 3. All Pools View: scene.getAll()
        // Iterates through EVERY pool in the scene.
        MultiPoolView getAll() {
            MultiPoolView view;
            view.pools.reserve(m_pools.size());
            for (auto& pool : m_pools) {
                view.pools.push_back(pool.get());
            }
            return view;
        }

        void registerName(size_t id, const std::string& name) {
            if (id < m_poolNames.size()) m_poolNames[id] = name;
        }

        const std::string& getName(size_t id) const {
            if (id < m_poolNames.size()) return m_poolNames[id];
            static const std::string unknown = "InvalidID";
            return unknown;
        }

        void clear() {
            for (auto& p : m_pools) p->clear();
        }

        // Raw access (for debuggers/loggers)
        const std::vector<std::unique_ptr<Pool>>& getRawPools() const {
            return m_pools;
        }

    private:
        std::vector<std::unique_ptr<Pool>> m_pools;
        std::vector<std::string> m_poolNames;
    };
}