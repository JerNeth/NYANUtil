#include <gtest/gtest.h>

import NYANAssert;
import std;

#undef assert
namespace nyan
{
    TEST(Assert, Test) {
        using namespace assert;
        constexpr auto testAssert = Assert< AssertionLevel::Enabled, AssertionExitMode::Abort, AssertionLogMode::Disabled, AssertionDebugMode::Disabled>{};
        EXPECT_EXIT({
            testAssert(false);
            }, testing::ExitedWithCode(3), "");

        constexpr auto testAssertMsgOnly = Assert< AssertionLevel::Enabled, AssertionExitMode::Abort, AssertionLogMode::MessageOnly, AssertionDebugMode::Disabled > {};
        EXPECT_EXIT({
            testAssertMsgOnly(false, "Hello");
            }, testing::ExitedWithCode(3), "Hello");

        constexpr auto testSourceLoc = Assert < AssertionLevel::Enabled, AssertionExitMode::Abort, AssertionLogMode::SourceLocation, AssertionDebugMode::Disabled > {};
        EXPECT_EXIT({
            testSourceLoc(false);
            }, testing::ExitedWithCode(3), "nyan::Assert");

        constexpr auto testAssertStackTrace = Assert < AssertionLevel::Enabled, AssertionExitMode::Abort, AssertionLogMode::StackTrace, AssertionDebugMode::Disabled > {};
        EXPECT_EXIT({
            testAssertStackTrace(false);
            }, testing::ExitedWithCode(3), "Test::Run");


        constexpr auto testAssertExit = Assert < AssertionLevel::Enabled, AssertionExitMode::Exit, AssertionLogMode::Disabled, AssertionDebugMode::Disabled > {};
        EXPECT_EXIT({
            testAssertExit(false);
            }, testing::ExitedWithCode(1), "");

        constexpr auto testAssertQuickExit = Assert < AssertionLevel::Enabled, AssertionExitMode::QuickExit, AssertionLogMode::Disabled, AssertionDebugMode::Disabled > {};
        EXPECT_EXIT({
            testAssertQuickExit(false);
            }, testing::ExitedWithCode(1), "");

        constexpr auto testAssertNoExit = Assert < AssertionLevel::Enabled, AssertionExitMode::Disabled, AssertionLogMode::Disabled, AssertionDebugMode::Disabled > {};
        EXPECT_NO_FATAL_FAILURE({
            testAssertNoExit(false);
            });
    }
    TEST(Assert, Lazy) {

        using namespace assert;
        constexpr auto testAssert = Assert < AssertionLevel::Enabled, AssertionExitMode::Disabled, AssertionLogMode::Disabled, AssertionDebugMode::Disabled > {};

        int flag = 0;

        EXPECT_NO_FATAL_FAILURE({
            testAssert([&]() {flag++; return false; });
            });

        EXPECT_EQ(flag, 1);

        flag = 0;

        constexpr auto testAssertDisabled = Assert < AssertionLevel::Disabled, AssertionExitMode::Disabled, AssertionLogMode::Disabled, AssertionDebugMode::Disabled>{};

        EXPECT_NO_FATAL_FAILURE({
            testAssertDisabled([&]() {flag++; return false; });
            });
        EXPECT_EQ(flag, 0);
        std::expected<uint32_t, uint32_t> a = std::unexpected{ 0u };

        constexpr auto testAssert2 = Assert < AssertionLevel::Enabled, AssertionExitMode::Abort, AssertionLogMode::Disabled, AssertionDebugMode::Disabled>{};

        EXPECT_EXIT({
            testAssert2(a);
            }, testing::ExitedWithCode(3), "");
    }
}