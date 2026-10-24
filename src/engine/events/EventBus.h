#pragma once
#include <vector>
#include <memory>
#include <algorithm>

namespace Engine::Events {

    // --- ID Generation ---
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

        EventQueue(std::size_t capacity = 1024) {
            // FIX: Use reserve, not the constructor initializer list
            events.reserve(capacity);
        }

        void clear() override {
            // This leaves the capacity intact for the next frame,
            // preventing expensive heap re-allocations!
            events.clear();
        }
    };

    class EventBus {
    public:
        using QueueList = std::vector<std::unique_ptr<IEventQueue>>;

        // FIX: Changed return type to void (or you could return EventQueue<T>&)
        template<typename T>
        void registerEvent(std::size_t capacity = 1024) {
            std::size_t id = getEventID<T>();

            if (id >= m_queues.size()) m_queues.resize(id + 1);
            if (!m_queues[id]) {
                m_queues[id] = std::make_unique<EventQueue<T>>(capacity);
            } else {
                // If already registered, just ensure capacity
                static_cast<EventQueue<T>*>(m_queues[id].get())->events.reserve(capacity);
            }
        }

        template <typename T, typename... TArgs>
        void publish(TArgs&&... args) {
            std::size_t id = getEventID<T>();

            // FIX: Must ensure the queue exists in case a system publishes
            // before anyone explicitly registered the event!
            if (id >= m_queues.size()) m_queues.resize(id + 1);
            if (!m_queues[id]) {
                m_queues[id] = std::make_unique<EventQueue<T>>();
            }

            auto model = static_cast<EventQueue<T>*>(m_queues[id].get());
            model->events.emplace_back(std::forward<TArgs>(args)...);
        }

        template <typename T>
        const std::vector<T>& get() const {
            std::size_t id = getEventID<T>();

            if (id >= m_queues.size() || !m_queues[id]) {
                 static const std::vector<T> empty;
                 return empty;
            }

            auto model = static_cast<EventQueue<T>*>(m_queues[id].get());
            return model->events;
        }

        void clear() {
            // Called at the end of the frame
            for (auto& queue : m_queues) {
                if (queue) queue->clear();
            }
        }

    private:
        QueueList m_queues;
    };
}