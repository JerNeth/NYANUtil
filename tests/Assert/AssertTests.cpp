import NYANAssert;


#include <gtest/gtest.h>
#undef assert
namespace nyan
{
    TEST(Assert, Test) {
        using namespace assert;
        constexpr auto testAssert = Assert<AssertionLevel::Enabled, AssertionExitMode::Abort, AssertionLogMode::Disabled>{};
        EXPECT_EXIT({
            testAssert(false);
            }, testing::ExitedWithCode(3), "");

        constexpr auto testAssertMsgOnly = Assert<AssertionLevel::Enabled, AssertionExitMode::Abort, AssertionLogMode::MessageOnly>{};
        EXPECT_EXIT({
            testAssertMsgOnly(false, "Hello");
            }, testing::ExitedWithCode(3), "Hello");

        constexpr auto testSourceLoc = Assert<AssertionLevel::Enabled, AssertionExitMode::Abort, AssertionLogMode::SourceLocation>{};
        EXPECT_EXIT({
            testSourceLoc(false);
            }, testing::ExitedWithCode(3), "nyan::Assert");

        constexpr auto testAssertStackTrace = Assert<AssertionLevel::Enabled, AssertionExitMode::Abort, AssertionLogMode::StackTrace>{};
        EXPECT_EXIT({
            testAssertStackTrace(false);
            }, testing::ExitedWithCode(3), "Test::Run");


        constexpr auto testAssertExit = Assert<AssertionLevel::Enabled, AssertionExitMode::Exit, AssertionLogMode::Disabled>{};
        EXPECT_EXIT({
            testAssertExit(false);
            }, testing::ExitedWithCode(1), "");

        constexpr auto testAssertQuickExit = Assert<AssertionLevel::Enabled, AssertionExitMode::QuickExit, AssertionLogMode::Disabled>{};
        EXPECT_EXIT({
            testAssertQuickExit(false);
            }, testing::ExitedWithCode(1), "");

        constexpr auto testAssertNoExit = Assert<AssertionLevel::Enabled, AssertionExitMode::Disabled, AssertionLogMode::Disabled>{};
        EXPECT_NO_FATAL_FAILURE({
            testAssertNoExit(false);
            });
    }
}