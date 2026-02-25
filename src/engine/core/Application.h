//
// Created by marcel on 7/31/25.
//
#pragma once



#include "glm/glm.hpp"
#include <string>
#include <memory>

namespace Engine::Core     { class Context;           }
namespace Engine::Core     { class IGame;             }



namespace Engine {
    class Application {

    public:
                                                            Application();
                                                            ~Application();

        bool                                                init(const std::string& title, std::size_t width, std::size_t height, glm::uint32 target_fps);
        void                                                run(Core::IGame &game);

        void                                                setRunning(const bool running)  { m_is_running = running;       };
        bool                                                isRunning()             const   { return m_is_running;          };

        Core::Context&                                      getContext()            const   { return *m_context;            };


    private:

        std::unique_ptr<Core::Context>                      m_context;


        bool                                                m_is_running;
        bool                                                m_quit_requested;
    };
}