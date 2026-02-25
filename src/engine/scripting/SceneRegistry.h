#pragma once
#include <sol/sol.hpp>
#include <functional>
#include <unordered_map>
#include <string>
#include <iostream>

// Include your GameObject definition
namespace Engine::Core { class GameObject; }

namespace Engine::Scripting {

    using SceneID = std::size_t;

    class SceneRegistry {
    public:

        static SceneRegistry& Get() {
            static SceneRegistry instance;
            return instance;
        }


        SceneRegistry(const SceneRegistry&) = delete;
        void operator=(const SceneRegistry&) = delete;


        bool registerScene(const std::string& name, SceneID id) {
            m_factories[name] = id;
            return true;
        }


        std::size_t getSceneID(const std::string& scene_name) {

            SceneID id =  m_factories[scene_name];
            return id;
        }

    private:
        // Private Constructor: Ensures no one can say "new SceneRegistry()"
        SceneRegistry() = default;
        ~SceneRegistry() = default;

        std::unordered_map<std::string, SceneID> m_factories;
    };
}