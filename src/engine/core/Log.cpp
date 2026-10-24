#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Engine::Core {

namespace {
std::shared_ptr<spdlog::logger> makeLogger(const std::string& name) {
    auto logger = spdlog::stdout_color_mt(name);

    // Pattern: [Time] [LoggerName]: Message
    // %^ and %$ mark the colored part of the message
    logger->set_pattern("%^[%T] %n: %v%$");
    logger->set_level(spdlog::level::trace);
    return logger;
}
}  // namespace

// A function-local static is created the first time the function runs (thread-safe since C++11).
// So the loggers exist whenever someone logs: before, they were created in Log::init() at the END of
// the Application constructor, and anything logging earlier (or a test, which has no Application)
// would have called a method on a null shared_ptr.
spdlog::logger& Log::core() {
    static const std::shared_ptr<spdlog::logger> logger = makeLogger("ENGINE");
    return *logger;
}

spdlog::logger& Log::client() {
    static const std::shared_ptr<spdlog::logger> logger = makeLogger("GAME");
    return *logger;
}

}  // namespace Engine::Core
