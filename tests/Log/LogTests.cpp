import NYANLog;

#include <gtest/gtest.h>

namespace Utility
{
    TEST(Log, LogToStdout) {
        testing::internal::CaptureStdout();
        std::string inputInfo = "Hello World from info";
        nyan::util::log::info().message(inputInfo);
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_EQ(inputInfo, output);

        std::string inputVerbose = "Hello World from verbose";
        nyan::util::log::verbose().message(inputVerbose);

        std::string inputWarning = "Hello World from warning";
        nyan::util::log::warning().message(inputWarning);

        std::string inputError = "Hello World from error";
        nyan::util::log::error().message(inputError);

        std::string inputCritical = "Hello World from critical";
        nyan::util::log::critical().message(inputCritical);

    }
}