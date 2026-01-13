#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Engine::Core {

std::shared_ptr<spdlog::logger> Log::s_core_logger;
std::shared_ptr<spdlog::logger> Log::s_client_logger;

void Log::init() {
    // Pattern: [Time] [LoggerName]: Message
    // %^ and %$ mark the colored part of the message
    spdlog::set_pattern("%^[%T] %n: %v%$");

    // 1. Core Logger (Engine)
    s_core_logger = spdlog::stdout_color_mt("ENGINE");
    s_core_logger->set_level(spdlog::level::trace);

    // 2. Client Logger (Game)
    s_client_logger = spdlog::stdout_color_mt("GAME");
    s_client_logger->set_level(spdlog::level::trace);
}

}  // namespace Engine::Core