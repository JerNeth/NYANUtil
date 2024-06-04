import NYANLog;

#include <gtest/gtest.h>
#include <array>

namespace Utility
{
    TEST(Log, LogToStdout) {
        testing::internal::CaptureStdout();
        std::string inputInfo = "Hello World from info";
        {
            nyan::util::log::info().message(inputInfo);
        }
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_EQ(inputInfo + "\n", output);

        std::string inputVerbose = "Hello World from verbose";
        nyan::util::log::verbose().message(inputVerbose);

        std::string inputWarning = "Hello World from warning";
        nyan::util::log::warning().message(inputWarning);

        std::string inputError = "Hello World from error";
        nyan::util::log::error().message(inputError);

        std::string inputCritical = "Hello World from critical";
        nyan::util::log::critical().message(inputCritical);

    }
    TEST(Log, LogFormat) {
        {
            testing::internal::CaptureStdout();
            std::string inputInfo = "Test";
            {
                nyan::util::log::info_message(inputInfo);
            }
            std::string output = testing::internal::GetCapturedStdout();
            EXPECT_EQ(inputInfo + "\n", output);
        }
        {
            testing::internal::CaptureStdout();
            std::string inputInfo = "Test";
            {
                nyan::util::log::info().format(inputInfo);
            }
            std::string output = testing::internal::GetCapturedStdout();
            EXPECT_EQ(inputInfo + "\n", output);
        }
        {
            testing::internal::CaptureStdout();
            std::string inputInfo ="[Instance] Found vulkan capable device: test\ndevice\n1.3.125.0";
            {
                auto logger = nyan::util::log::info();
                logger.format("[Instance] Found vulkan capable device: {}\n{}\n{}.{}.{}.{}", "test", "device",
                   1, 3,
                    125,0);
            }
            {
                auto logger = nyan::util::log::info();
                logger.format("[Instance] Found vulkan capable device: {}\n{}\n{}.{}.{}.{}", "test", "device",
                    1, 3,
                    125, 0);
            }
            std::string output = testing::internal::GetCapturedStdout();
            EXPECT_EQ(inputInfo + "\n" + inputInfo + "\n", output);
        }
    }
    TEST(Log, LogLocation) {

        testing::internal::CaptureStdout();
        {
            nyan::util::log::info().location();
        }
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_TRUE(!output.empty());
    }
    TEST(Log, LogLocationFormatCombo) {

        testing::internal::CaptureStdout();
        {

            nyan::util::log::info().location().format("{}", "test");
        }
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_TRUE(!output.empty());
    }
    TEST(Log, LogLambda) {

        auto test = []() {
            nyan::util::log::info().location().format("{}", "test");
            };
        auto test2 = [](auto logger) {
            logger.location().format("{}", "test");
            };
        testing::internal::CaptureStdout();
        {
            test();
            test2(nyan::util::log::info());
        }
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_TRUE(!output.empty());
    }
    TEST(Log, LogColor) {

        testing::internal::CaptureStdout();
        {
            std::array<uint8_t, 3> color{ 255, 0, 0 };
            int a = 0;
            nyan::util::log::info().location().format(color, "{} {}", "test", a);
        }
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_TRUE(!output.empty());
    }
}