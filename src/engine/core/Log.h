#pragma once

// We still include spdlog in release so we can log errors,
// but we might want to strip it entirely for a "Final Master" build.
#include <memory>

#include <spdlog/spdlog.h>  // only what the macros below need, not the whole pch.h

namespace Engine::Core {

// Two loggers: ENGINE for the engine, GAME for the game. Use the macros below, not these directly.
// Messages use fmt syntax: ENGINE_WARN("Failed to load {}: {}", path, SDL_GetError());
class Log {
public:
    static spdlog::logger& core();    // created on first use: no init() to forget or call too late
    static spdlog::logger& client();
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
    #define ENGINE_WARN(...)     ::Engine::Core::Log::core().warn(__VA_ARGS__)
    #define ENGINE_ERROR(...)    ::Engine::Core::Log::core().error(__VA_ARGS__)
    #define ENGINE_FATAL(...)    ::Engine::Core::Log::core().critical(__VA_ARGS__)

    // Client Logging
    #define LOG_TRACE(...)
    #define LOG_INFO(...)
    #define LOG_WARN(...)        ::Engine::Core::Log::client().warn(__VA_ARGS__)
    #define LOG_ERROR(...)       ::Engine::Core::Log::client().error(__VA_ARGS__)
    #define LOG_FATAL(...)       ::Engine::Core::Log::client().critical(__VA_ARGS__)

#else

    // DEBUG MODE (All logs active)
    #define ENGINE_TRACE(...)    ::Engine::Core::Log::core().trace(__VA_ARGS__)
    #define ENGINE_INFO(...)     ::Engine::Core::Log::core().info(__VA_ARGS__)
    #define ENGINE_WARN(...)     ::Engine::Core::Log::core().warn(__VA_ARGS__)
    #define ENGINE_ERROR(...)    ::Engine::Core::Log::core().error(__VA_ARGS__)
    #define ENGINE_FATAL(...)    ::Engine::Core::Log::core().critical(__VA_ARGS__)

    #define LOG_TRACE(...)       ::Engine::Core::Log::client().trace(__VA_ARGS__)
    #define LOG_INFO(...)        ::Engine::Core::Log::client().info(__VA_ARGS__)
    #define LOG_WARN(...)        ::Engine::Core::Log::client().warn(__VA_ARGS__)
    #define LOG_ERROR(...)       ::Engine::Core::Log::client().error(__VA_ARGS__)
    #define LOG_FATAL(...)       ::Engine::Core::Log::client().critical(__VA_ARGS__)

#endif