#pragma once
#include <vector>
#include <memory>
#include <algorithm>

namespace Engine::Events {

    // --- ID Generation (Unchanged) ---
    struct EventFamily {
        static size_t identifier() {
            static size_t value = 0;
            return value++;
        }
    };

    template<typename T>
    size_t getEventID() {
        static const size_t value = EventFamily::identifier();
        return value;
    }

    // --- Interfaces ---
    struct IEventQueue {
        virtual ~IEventQueue() = default;
        virtual void clear() = 0;
    };

    template<typename T>
    struct EventQueue : public IEventQueue {
        std::vector<T> events;

        EventQueue(std::size_t capacity = 1024) : events(capacity) {}
        
        void clear() override {
            events.clear(); 
            // Optional: events.reserve(100); 
            // Optimization: If you know you always have ~100 events, 
            // keeping capacity prevents re-allocation next frame.
        }
    };

    class EventBus {
    public:
        // OPTIMIZATION: Use unique_ptr because EventBus owns the memory exclusively
        using QueueList = std::vector<std::unique_ptr<IEventQueue>>;


        template<typename T>
        T& registerEvent(std::size_t capacity = 1024) {

            std::size_t id = getEventID<T>();

            if (id >= m_queues.size()) m_queues.resize(id + 1);
            if (!m_queues[id]) {
                m_queues[id] = std::make_unique<EventQueue<T>>(capacity);
            }
            return m_queues[id];
        }

        template <typename T, typename... TArgs>
        void publish(TArgs&&... args) {
            std::size_t id = getEventID<T>();

            // 3. Fast Access
            // We use static_cast because we trust our ID system (safe & fast)
            auto model = static_cast<EventQueue<T>*>(m_queues[id].get());
            model->events.emplace_back(std::forward<TArgs>(args)...);
        }

        template <typename T>
        const std::vector<T>& get() const {
            // Note: Returning const reference is safer so systems don't mess with the list
            std::size_t id = getEventID<T>();

            // If requested before any were published, return empty (safe)
            if (id >= m_queues.size() || !m_queues[id]) {
                 static const std::vector<T> empty;
                 return empty;
            }

            auto model = static_cast<EventQueue<T>*>(m_queues[id].get());
            return model->events;
        }

        void clear() {
            for (auto& queue : m_queues) {
                if (queue) queue->clear();
            }
        }

    private:
        QueueList m_queues;
    };
}