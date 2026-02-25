//
// Created by marcel on 1/31/26.
//

#pragma once

#include <memory>
#include <vector>
#include <cassert>

namespace Engine::Core {

    // --- ID Generator (Unchanged) ---
    struct ServiceFamily {
        static size_t identifier() {
            static size_t value = 0;
            return value++;
        }
    };

    template<typename T>
    size_t getServiceID() {
        static const size_t value = ServiceFamily::identifier();
        return value;
    }
    // --------------------------------

    class Context {
    public:
        Context() {
            m_services.resize(64);
        }

        // --- NEW: Variadic Register Function ---
        // usage: registerService<Scene>();
        // usage: registerService<AssetManager>(renderer_ptr);
        template<typename T, typename... Args>
        T& registerService(Args&&... args) {

            size_t id = getServiceID<T>();

            // Resize if needed
            if (id >= m_services.size()) {
                m_services.resize(id + 1);
            }

            // Construct T in-place using the forwarded arguments
            // We use make_shared directly for efficiency
            auto newService = std::make_shared<T>(std::forward<Args>(args)...);

            // Store as shared_ptr<void> (keeps the custom deleter alive)
            m_services[id] = newService;

            return *newService;
        }

        template<typename T>
        T& get() {
            size_t id = getServiceID<T>();
            assert(id < m_services.size() && m_services[id] != nullptr && "Service not registered!");

            return *static_cast<T*>(m_services[id].get());
        }

    private:
        std::vector<std::shared_ptr<void>> m_services;
    };
}