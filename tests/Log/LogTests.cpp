import NYANLog;

#include <gtest/gtest.h>

namespace Utility
{
    TEST(Log, LogToStdout) {
        testing::internal::CaptureStdout();
        std::string input = "Hello World";
        nyan::util::log::Logger::info().message(input);
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_EQ(input, output);
    }
}