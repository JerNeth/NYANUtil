import NYANLog;

#include <gtest/gtest.h>
#include <array>

namespace nyan
{
    TEST(Log, LogToStdout) {
        testing::internal::CaptureStdout();
        std::string inputInfo = "Hello World from info";
        {
            log::info().message(inputInfo);
        }
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_EQ(inputInfo + "\n", output);

        std::string inputVerbose = "Hello World from verbose";
        log::verbose().message(inputVerbose);

        std::string inputWarning = "Hello World from warning";
        log::warning().message(inputWarning);

        std::string inputError = "Hello World from error";
        log::error().message(inputError);

        std::string inputCritical = "Hello World from critical";
        log::critical().message(inputCritical);

    }
    TEST(Log, LogFormat) {
        {
            testing::internal::CaptureStdout();
            std::string inputInfo = "Test";
            {
                log::info().message(inputInfo);
            }
            std::string output = testing::internal::GetCapturedStdout();
            EXPECT_EQ(inputInfo + "\n", output);
        }
        {
            testing::internal::CaptureStdout();
            std::string inputInfo = "Test";
            {
                log::info().format_runtime(inputInfo);
            }
            std::string output = testing::internal::GetCapturedStdout();
            EXPECT_EQ(inputInfo + "\n", output);
        }
        {
            testing::internal::CaptureStdout();
            std::string inputInfo ="[Instance] Found vulkan capable device: test\ndevice\n1.3.125.0";
            {
                auto logger = log::info();
                logger.format("[Instance] Found vulkan capable device: {}\n{}\n{}.{}.{}.{}", "test", "device",
                   1, 3,
                    125,0);
            }
            {
                auto logger = log::info();
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
            log::info().location();
        }
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_TRUE(!output.empty());
    }
    TEST(Log, LogLocationFormatCombo) {

        testing::internal::CaptureStdout();
        {

            log::info().location().format("{}", "test");
        }
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_TRUE(!output.empty());
    }
    TEST(Log, LogLambda) {

        auto test = []() {
            log::info().location().format("{}", "test");
            };
        auto test2 = [](auto logger) {
            logger.location().format("{}", "test");

            };
        testing::internal::CaptureStdout();
        {
            test();
            test2(log::info());
        }
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_TRUE(!output.empty());
    }
    TEST(Log, LogColor) {

        testing::internal::CaptureStdout();
        {
            std::array<uint8_t, 3> color{ 255, 0, 0 };
            int a = 0;
            log::info().location().format_color(color, "{} {}", "test", a);
        }
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_TRUE(!output.empty());
    }
    TEST(Log, LogFormatLambdaTests) {

        auto lambda = [&](auto logger) {
            std::array<uint8_t, 3> color{ 255, 0, 0 };
            int a = 0;
            logger.format("Hello {}", "World");
            };
        testing::internal::CaptureStdout();
        {
            lambda(log::info().location());
        }
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_TRUE(!output.empty());
    }
    TEST(Log, LogFormatClassLambdaTests) {
        class Test {
        public:
            void function(uint32_t value) {
                auto lambda = [&](auto&& logger, const auto& message, auto value2) {
                    std::array<uint8_t, 3> color{ 255, 0, 0 };
                    int a = 0;
                    logger.format_runtime(message, "World", value2);
                    };
                auto str = "Hello {} {}";
                lambda(log::info().location(), str, value);
            }
        };
        testing::internal::CaptureStdout();
        {
            Test t;
            t.function(32);
        }
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_TRUE(!output.empty());
    }
    TEST(Log, LogStacktrace) {

        testing::internal::CaptureStdout();
        {
            log::warning().stacktrace();
        }
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_TRUE(!output.empty());
    }
}