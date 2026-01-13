#pragma once

#include <memory>
// We still include spdlog in release so we can log errors,
// but we might want to strip it entirely for a "Final Master" build.
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Engine::Core {

class Log {
public:
    static void init();
    static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_core_logger; }
    static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_client_logger; }

private:
    static std::shared_ptr<spdlog::logger> s_core_logger;
    static std::shared_ptr<spdlog::logger> s_client_logger;
};

} // namespace Engine::Core

// -------------------------------------------------------------------------
// LOGGING MACROS
// -------------------------------------------------------------------------

// If NOT Debug mode (aka Release), we replace TRACE/INFO calls with empty code.
#if defined(NDEBUG)

    // RELEASE MODE
    // (We strip out Info/Trace to save CPU cycles)
    #define ENGINE_TRACE(...)
    #define ENGINE_INFO(...)
    // We KEEP Warnings/Errors because they are critical
    #define ENGINE_WARN(...)     ::Engine::Core::Log::getCoreLogger()->warn(__VA_ARGS__)
    #define ENGINE_ERROR(...)    ::Engine::Core::Log::getCoreLogger()->error(__VA_ARGS__)
    #define ENGINE_FATAL(...)    ::Engine::Core::Log::getCoreLogger()->critical(__VA_ARGS__)

    // Client Logging
    #define LOG_TRACE(...)
    #define LOG_INFO(...)
    #define LOG_WARN(...)        ::Engine::Core::Log::getClientLogger()->warn(__VA_ARGS__)
    #define LOG_ERROR(...)       ::Engine::Core::Log::getClientLogger()->error(__VA_ARGS__)
    #define LOG_FATAL(...)       ::Engine::Core::Log::getClientLogger()->critical(__VA_ARGS__)

#else

    // DEBUG MODE (All logs active)
    #define ENGINE_TRACE(...)    ::Engine::Core::Log::getCoreLogger()->trace(__VA_ARGS__)
    #define ENGINE_INFO(...)     ::Engine::Core::Log::getCoreLogger()->info(__VA_ARGS__)
    #define ENGINE_WARN(...)     ::Engine::Core::Log::getCoreLogger()->warn(__VA_ARGS__)
    #define ENGINE_ERROR(...)    ::Engine::Core::Log::getCoreLogger()->error(__VA_ARGS__)
    #define ENGINE_FATAL(...)    ::Engine::Core::Log::getCoreLogger()->critical(__VA_ARGS__)

    #define LOG_TRACE(...)       ::Engine::Core::Log::getClientLogger()->trace(__VA_ARGS__)
    #define LOG_INFO(...)        ::Engine::Core::Log::getClientLogger()->info(__VA_ARGS__)
    #define LOG_WARN(...)        ::Engine::Core::Log::getClientLogger()->warn(__VA_ARGS__)
    #define LOG_ERROR(...)       ::Engine::Core::Log::getClientLogger()->error(__VA_ARGS__)
    #define LOG_FATAL(...)       ::Engine::Core::Log::getClientLogger()->critical(__VA_ARGS__)

#endif