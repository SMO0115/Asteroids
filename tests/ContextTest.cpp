#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "engine/core/Context.h"

using namespace Engine::Core;

namespace {
// Fake services that write into a log when they are destroyed
struct FakeRenderer {
    explicit FakeRenderer(std::vector<std::string>& log) : m_log(log) {}
    ~FakeRenderer() { m_log.push_back("Renderer"); }
    std::vector<std::string>& m_log;
};

struct FakeAssets {
    explicit FakeAssets(std::vector<std::string>& log) : m_log(log) {}
    ~FakeAssets() { m_log.push_back("Assets"); }
    std::vector<std::string>& m_log;
};
}  // namespace

TEST(ContextTest, DestroysServicesInReverseRegistrationOrder) {
    std::vector<std::string> log;
    {
        Context ctx;
        ctx.registerService<FakeRenderer>(log);
        ctx.registerService<FakeAssets>(log);  // uses the renderer, so it must die first
    }

    EXPECT_EQ(log, (std::vector<std::string>{"Assets", "Renderer"}));
}

TEST(ContextTest, GetReturnsTheRegisteredInstance) {
    std::vector<std::string> log;
    Context                  ctx;
    FakeRenderer&            registered = ctx.registerService<FakeRenderer>(log);

    EXPECT_EQ(&ctx.get<FakeRenderer>(), &registered);
}
