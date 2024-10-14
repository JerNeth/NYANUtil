import NYANAssert;


#include <gtest/gtest.h>
#undef assert
namespace nyan
{
    TEST(Assert, Test) {
        using namespace assert;
        constexpr auto testAssert = Assert< static_cast<size_t>(AssertionLevel::Enabled), static_cast<size_t>(AssertionExitMode::Abort), static_cast<size_t>(AssertionLogMode::Disabled)>{};
        EXPECT_EXIT({
            testAssert(false);
            }, testing::ExitedWithCode(3), "");

        constexpr auto testAssertMsgOnly = Assert< static_cast<size_t>(AssertionLevel::Enabled), static_cast<size_t>(AssertionExitMode::Abort), static_cast<size_t>(AssertionLogMode::MessageOnly )> {};
        EXPECT_EXIT({
            testAssertMsgOnly(false, "Hello");
            }, testing::ExitedWithCode(3), "Hello");

        constexpr auto testSourceLoc = Assert < static_cast<size_t>(AssertionLevel::Enabled), static_cast<size_t>(AssertionExitMode::Abort), static_cast<size_t>(AssertionLogMode::SourceLocation )> {};
        EXPECT_EXIT({
            testSourceLoc(false);
            }, testing::ExitedWithCode(3), "nyan::Assert");

        constexpr auto testAssertStackTrace = Assert < static_cast<size_t>(AssertionLevel::Enabled), static_cast<size_t>(AssertionExitMode::Abort), static_cast<size_t>(AssertionLogMode::StackTrace )> {};
        EXPECT_EXIT({
            testAssertStackTrace(false);
            }, testing::ExitedWithCode(3), "Test::Run");


        constexpr auto testAssertExit = Assert < static_cast<size_t>(AssertionLevel::Enabled), static_cast<size_t>(AssertionExitMode::Exit), static_cast<size_t>(AssertionLogMode::Disabled )> {};
        EXPECT_EXIT({
            testAssertExit(false);
            }, testing::ExitedWithCode(1), "");

        constexpr auto testAssertQuickExit = Assert < static_cast<size_t>(AssertionLevel::Enabled), static_cast<size_t>(AssertionExitMode::QuickExit), static_cast<size_t>(AssertionLogMode::Disabled )> {};
        EXPECT_EXIT({
            testAssertQuickExit(false);
            }, testing::ExitedWithCode(1), "");

        constexpr auto testAssertNoExit = Assert < static_cast<size_t>(AssertionLevel::Enabled), static_cast<size_t>(AssertionExitMode::Disabled), static_cast<size_t>(AssertionLogMode::Disabled )> {};
        EXPECT_NO_FATAL_FAILURE({
            testAssertNoExit(false);
            });
    }
    TEST(Assert, Lazy) {

        using namespace assert;
        constexpr auto testAssert = Assert < static_cast<size_t>(AssertionLevel::Enabled), static_cast<size_t>(AssertionExitMode::Disabled), static_cast<size_t>(AssertionLogMode::Disabled )> {};

        int flag = 0;

        EXPECT_NO_FATAL_FAILURE({
            testAssert([&]() {flag++; return false; });
            });

        EXPECT_EQ(flag, 1);

        flag = 0;

        constexpr auto testAssertDisabled = Assert < static_cast<size_t>(AssertionLevel::Disabled), static_cast<size_t>(AssertionExitMode::Disabled), static_cast<size_t>(AssertionLogMode::Disabled)>{};

        EXPECT_NO_FATAL_FAILURE({
            testAssertDisabled([&]() {flag++; return false; });
            });
        EXPECT_EQ(flag, 0);
    }
}