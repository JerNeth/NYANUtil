import NYANLog;

#include <gtest/gtest.h>

namespace Utility
{
    TEST(Log, LogToStdout) {
        testing::internal::CaptureStdout();
        std::string input = "Hello World";
        Utility::Logger::info().message(input);
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_EQ(input, output);
    }
}