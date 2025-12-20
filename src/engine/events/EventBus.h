//
// Created by marcel on 11/16/25.
//

#pragma once
#include <vector>
#include <tuple>
#include <utility>


template <typename... Events>
class EventBus {
public:

    template<typename T, typename... TArgs>
    void publish(TArgs&&... args) {
        std::get<std::vector<T>>(m_queues).emplace_back(std::forward<TArgs>(args)...);
    }

    template<typename T>
    std::vector<T>& getEvents() {
        return std::get<std::vector<T>>(m_queues);
    }

    void clear() {
        std::apply([](auto&... queue) { (queue.clear(), ...); }, m_queues);
    }

private:
    std::tuple<std::vector<Events>...> m_queues;
};