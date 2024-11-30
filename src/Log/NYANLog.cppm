module;

//#include <array>
//#include <format>
//#include <iostream>
//#include <source_location>
//#if __cpp_lib_stacktrace >= 202011L
//#include <stacktrace>
//#endif
//#include <string>
//#include <string_view>

#include <version>
#include <cstdio>

#ifdef WIN32
#define NOMINMAX
#include <Windows.h>
#endif

export module NYANLog;
import std;

namespace impl
{
    constexpr std::array consoleColorValues
    {
        std::array<std::uint8_t, 3>{12, 12, 12 },
        std::array<std::uint8_t, 3>{ 197, 15, 31 },
        std::array<std::uint8_t, 3>{ 19, 161, 14 },
        std::array<std::uint8_t, 3>{ 193, 156, 0 },
        std::array<std::uint8_t, 3>{ 0, 55, 218 },
        std::array<std::uint8_t, 3>{ 136, 23, 152 },
        std::array<std::uint8_t, 3>{ 58, 150, 221 },
        std::array<std::uint8_t, 3>{ 204, 204, 204 },
        std::array<std::uint8_t, 3>{ 118, 118, 118 },
        std::array<std::uint8_t, 3>{ 231, 72, 86 },
        std::array<std::uint8_t, 3>{ 22, 198, 12 },
        std::array<std::uint8_t, 3>{ 249, 241, 165 },
        std::array<std::uint8_t, 3>{ 59, 120, 255 },
        std::array<std::uint8_t, 3>{ 180, 0, 158 },
        std::array<std::uint8_t, 3>{ 97, 214, 214 },
        std::array<std::uint8_t, 3>{ 242, 242, 242 },
        std::array<std::uint8_t, 3>{ 0, 0, 0 },
        std::array<std::uint8_t, 3>{ 0, 0, 95 },
        std::array<std::uint8_t, 3>{ 0, 0, 135 },
        std::array<std::uint8_t, 3>{ 0, 0, 175 },
        std::array<std::uint8_t, 3>{ 0, 0, 215 },
        std::array<std::uint8_t, 3>{ 0, 0, 255 },
        std::array<std::uint8_t, 3>{ 0, 95, 0 },
        std::array<std::uint8_t, 3>{ 0, 95, 95 },
        std::array<std::uint8_t, 3>{ 0, 95, 135 },
        std::array<std::uint8_t, 3>{ 0, 95, 175 },
        std::array<std::uint8_t, 3>{ 0, 95, 215 },
        std::array<std::uint8_t, 3>{ 0, 95, 255 },
        std::array<std::uint8_t, 3>{ 0, 135, 0 },
        std::array<std::uint8_t, 3>{ 0, 135, 95 },
        std::array<std::uint8_t, 3>{ 0, 135, 135 },
        std::array<std::uint8_t, 3>{ 0, 135, 175 },
        std::array<std::uint8_t, 3>{ 0, 135, 215 },
        std::array<std::uint8_t, 3>{ 0, 135, 255 },
        std::array<std::uint8_t, 3>{ 0, 175, 0 },
        std::array<std::uint8_t, 3>{ 0, 175, 95 },
        std::array<std::uint8_t, 3>{ 0, 175, 135 },
        std::array<std::uint8_t, 3>{ 0, 175, 175 },
        std::array<std::uint8_t, 3>{ 0, 175, 215 },
        std::array<std::uint8_t, 3>{ 0, 175, 255 },
        std::array<std::uint8_t, 3>{ 0, 215, 0 },
        std::array<std::uint8_t, 3>{ 0, 215, 95 },
        std::array<std::uint8_t, 3>{ 0, 215, 135 },
        std::array<std::uint8_t, 3>{ 0, 215, 175 },
        std::array<std::uint8_t, 3>{ 0, 215, 215 },
        std::array<std::uint8_t, 3>{ 0, 215, 255 },
        std::array<std::uint8_t, 3>{ 0, 255, 0 },
        std::array<std::uint8_t, 3>{ 0, 255, 95 },
        std::array<std::uint8_t, 3>{ 0, 255, 135 },
        std::array<std::uint8_t, 3>{ 0, 255, 175 },
        std::array<std::uint8_t, 3>{ 0, 255, 215 },
        std::array<std::uint8_t, 3>{ 0, 255, 255 },
        std::array<std::uint8_t, 3>{ 95, 0, 0 },
        std::array<std::uint8_t, 3>{ 95, 0, 95 },
        std::array<std::uint8_t, 3>{ 95, 0, 135 },
        std::array<std::uint8_t, 3>{ 95, 0, 175 },
        std::array<std::uint8_t, 3>{ 95, 0, 215 },
        std::array<std::uint8_t, 3>{ 95, 0, 255 },
        std::array<std::uint8_t, 3>{ 95, 95, 0 },
        std::array<std::uint8_t, 3>{ 95, 95, 95 },
        std::array<std::uint8_t, 3>{ 95, 95, 135 },
        std::array<std::uint8_t, 3>{ 95, 95, 175 },
        std::array<std::uint8_t, 3>{ 95, 95, 215 },
        std::array<std::uint8_t, 3>{ 95, 95, 255 },
        std::array<std::uint8_t, 3>{ 95, 135, 0 },
        std::array<std::uint8_t, 3>{ 95, 135, 95 },
        std::array<std::uint8_t, 3>{ 95, 135, 135 },
        std::array<std::uint8_t, 3>{ 95, 135, 175 },
        std::array<std::uint8_t, 3>{ 95, 135, 215 },
        std::array<std::uint8_t, 3>{ 95, 135, 255 },
        std::array<std::uint8_t, 3>{ 95, 175, 0 },
        std::array<std::uint8_t, 3>{ 95, 175, 95 },
        std::array<std::uint8_t, 3>{ 95, 175, 135 },
        std::array<std::uint8_t, 3>{ 95, 175, 175 },
        std::array<std::uint8_t, 3>{ 95, 175, 215 },
        std::array<std::uint8_t, 3>{ 95, 175, 255 },
        std::array<std::uint8_t, 3>{ 95, 215, 0 },
        std::array<std::uint8_t, 3>{ 95, 215, 95 },
        std::array<std::uint8_t, 3>{ 95, 215, 135 },
        std::array<std::uint8_t, 3>{ 95, 215, 175 },
        std::array<std::uint8_t, 3>{ 95, 215, 215 },
        std::array<std::uint8_t, 3>{ 95, 215, 255 },
        std::array<std::uint8_t, 3>{ 95, 255, 0 },
        std::array<std::uint8_t, 3>{ 95, 255, 95 },
        std::array<std::uint8_t, 3>{ 95, 255, 135 },
        std::array<std::uint8_t, 3>{ 95, 255, 175 },
        std::array<std::uint8_t, 3>{ 95, 255, 215 },
        std::array<std::uint8_t, 3>{ 95, 255, 255 },
        std::array<std::uint8_t, 3>{ 135, 0, 0 },
        std::array<std::uint8_t, 3>{ 135, 0, 95 },
        std::array<std::uint8_t, 3>{ 135, 0, 135 },
        std::array<std::uint8_t, 3>{ 135, 0, 175 },
        std::array<std::uint8_t, 3>{ 135, 0, 215 },
        std::array<std::uint8_t, 3>{ 135, 0, 255 },
        std::array<std::uint8_t, 3>{ 135, 95, 0 },
        std::array<std::uint8_t, 3>{ 135, 95, 95 },
        std::array<std::uint8_t, 3>{ 135, 95, 135 },
        std::array<std::uint8_t, 3>{ 135, 95, 175 },
        std::array<std::uint8_t, 3>{ 135, 95, 215 },
        std::array<std::uint8_t, 3>{ 135, 95, 255 },
        std::array<std::uint8_t, 3>{ 135, 135, 0 },
        std::array<std::uint8_t, 3>{ 135, 135, 95 },
        std::array<std::uint8_t, 3>{ 135, 135, 135 },
        std::array<std::uint8_t, 3>{ 135, 135, 175 },
        std::array<std::uint8_t, 3>{ 135, 135, 215 },
        std::array<std::uint8_t, 3>{ 135, 135, 255 },
        std::array<std::uint8_t, 3>{ 135, 175, 0 },
        std::array<std::uint8_t, 3>{ 135, 175, 95 },
        std::array<std::uint8_t, 3>{ 135, 175, 135 },
        std::array<std::uint8_t, 3>{ 135, 175, 175 },
        std::array<std::uint8_t, 3>{ 135, 175, 215 },
        std::array<std::uint8_t, 3>{ 135, 175, 255 },
        std::array<std::uint8_t, 3>{ 135, 215, 0 },
        std::array<std::uint8_t, 3>{ 135, 215, 95 },
        std::array<std::uint8_t, 3>{ 135, 215, 135 },
        std::array<std::uint8_t, 3>{ 135, 215, 175 },
        std::array<std::uint8_t, 3>{ 135, 215, 215 },
        std::array<std::uint8_t, 3>{ 135, 215, 255 },
        std::array<std::uint8_t, 3>{ 135, 255, 0 },
        std::array<std::uint8_t, 3>{ 135, 255, 95 },
        std::array<std::uint8_t, 3>{ 135, 255, 135 },
        std::array<std::uint8_t, 3>{ 135, 255, 175 },
        std::array<std::uint8_t, 3>{ 135, 255, 215 },
        std::array<std::uint8_t, 3>{ 135, 255, 255 },
        std::array<std::uint8_t, 3>{ 175, 0, 0 },
        std::array<std::uint8_t, 3>{ 175, 0, 95 },
        std::array<std::uint8_t, 3>{ 175, 0, 135 },
        std::array<std::uint8_t, 3>{ 175, 0, 175 },
        std::array<std::uint8_t, 3>{ 175, 0, 215 },
        std::array<std::uint8_t, 3>{ 175, 0, 255 },
        std::array<std::uint8_t, 3>{ 175, 95, 0 },
        std::array<std::uint8_t, 3>{ 175, 95, 95 },
        std::array<std::uint8_t, 3>{ 175, 95, 135 },
        std::array<std::uint8_t, 3>{ 175, 95, 175 },
        std::array<std::uint8_t, 3>{ 175, 95, 215 },
        std::array<std::uint8_t, 3>{ 175, 95, 255 },
        std::array<std::uint8_t, 3>{ 175, 135, 0 },
        std::array<std::uint8_t, 3>{ 175, 135, 95 },
        std::array<std::uint8_t, 3>{ 175, 135, 135 },
        std::array<std::uint8_t, 3>{ 175, 135, 175 },
        std::array<std::uint8_t, 3>{ 175, 135, 215 },
        std::array<std::uint8_t, 3>{ 175, 135, 255 },
        std::array<std::uint8_t, 3>{ 175, 175, 0 },
        std::array<std::uint8_t, 3>{ 175, 175, 95 },
        std::array<std::uint8_t, 3>{ 175, 175, 135 },
        std::array<std::uint8_t, 3>{ 175, 175, 175 },
        std::array<std::uint8_t, 3>{ 175, 175, 215 },
        std::array<std::uint8_t, 3>{ 175, 175, 255 },
        std::array<std::uint8_t, 3>{ 175, 215, 0 },
        std::array<std::uint8_t, 3>{ 175, 215, 95 },
        std::array<std::uint8_t, 3>{ 175, 215, 135 },
        std::array<std::uint8_t, 3>{ 175, 215, 175 },
        std::array<std::uint8_t, 3>{ 175, 215, 215 },
        std::array<std::uint8_t, 3>{ 175, 215, 255 },
        std::array<std::uint8_t, 3>{ 175, 255, 0 },
        std::array<std::uint8_t, 3>{ 175, 255, 95 },
        std::array<std::uint8_t, 3>{ 175, 255, 135 },
        std::array<std::uint8_t, 3>{ 175, 255, 175 },
        std::array<std::uint8_t, 3>{ 175, 255, 215 },
        std::array<std::uint8_t, 3>{ 175, 255, 255 },
        std::array<std::uint8_t, 3>{ 215, 0, 0 },
        std::array<std::uint8_t, 3>{ 215, 0, 95 },
        std::array<std::uint8_t, 3>{ 215, 0, 135 },
        std::array<std::uint8_t, 3>{ 215, 0, 175 },
        std::array<std::uint8_t, 3>{ 215, 0, 215 },
        std::array<std::uint8_t, 3>{ 215, 0, 255 },
        std::array<std::uint8_t, 3>{ 215, 95, 0 },
        std::array<std::uint8_t, 3>{ 215, 95, 95 },
        std::array<std::uint8_t, 3>{ 215, 95, 135 },
        std::array<std::uint8_t, 3>{ 215, 95, 175 },
        std::array<std::uint8_t, 3>{ 215, 95, 215 },
        std::array<std::uint8_t, 3>{ 215, 95, 255 },
        std::array<std::uint8_t, 3>{ 215, 135, 0 },
        std::array<std::uint8_t, 3>{ 215, 135, 95 },
        std::array<std::uint8_t, 3>{ 215, 135, 135 },
        std::array<std::uint8_t, 3>{ 215, 135, 175 },
        std::array<std::uint8_t, 3>{ 215, 135, 215 },
        std::array<std::uint8_t, 3>{ 215, 135, 255 },
        std::array<std::uint8_t, 3>{ 223, 175, 0 },
        std::array<std::uint8_t, 3>{ 223, 175, 95 },
        std::array<std::uint8_t, 3>{ 223, 175, 135 },
        std::array<std::uint8_t, 3>{ 223, 175, 175 },
        std::array<std::uint8_t, 3>{ 223, 175, 215 },
        std::array<std::uint8_t, 3>{ 223, 175, 255 },
        std::array<std::uint8_t, 3>{ 223, 215, 0 },
        std::array<std::uint8_t, 3>{ 223, 215, 95 },
        std::array<std::uint8_t, 3>{ 223, 215, 135 },
        std::array<std::uint8_t, 3>{ 223, 215, 175 },
        std::array<std::uint8_t, 3>{ 223, 215, 215 },
        std::array<std::uint8_t, 3>{ 223, 215, 255 },
        std::array<std::uint8_t, 3>{ 223, 255, 0 },
        std::array<std::uint8_t, 3>{ 223, 255, 95 },
        std::array<std::uint8_t, 3>{ 223, 255, 135 },
        std::array<std::uint8_t, 3>{ 223, 255, 175 },
        std::array<std::uint8_t, 3>{ 223, 255, 215 },
        std::array<std::uint8_t, 3>{ 223, 255, 255 },
        std::array<std::uint8_t, 3>{ 255, 0, 0 },
        std::array<std::uint8_t, 3>{ 255, 0, 95 },
        std::array<std::uint8_t, 3>{ 255, 0, 135 },
        std::array<std::uint8_t, 3>{ 255, 0, 175 },
        std::array<std::uint8_t, 3>{ 255, 0, 215 },
        std::array<std::uint8_t, 3>{ 255, 0, 255 },
        std::array<std::uint8_t, 3>{ 255, 95, 0 },
        std::array<std::uint8_t, 3>{ 255, 95, 95 },
        std::array<std::uint8_t, 3>{ 255, 95, 135 },
        std::array<std::uint8_t, 3>{ 255, 95, 175 },
        std::array<std::uint8_t, 3>{ 255, 95, 215 },
        std::array<std::uint8_t, 3>{ 255, 95, 255 },
        std::array<std::uint8_t, 3>{ 255, 135, 0 },
        std::array<std::uint8_t, 3>{ 255, 135, 95 },
        std::array<std::uint8_t, 3>{ 255, 135, 135 },
        std::array<std::uint8_t, 3>{ 255, 135, 175 },
        std::array<std::uint8_t, 3>{ 255, 135, 215 },
        std::array<std::uint8_t, 3>{ 255, 135, 255 },
        std::array<std::uint8_t, 3>{ 255, 175, 0 },
        std::array<std::uint8_t, 3>{ 255, 175, 95 },
        std::array<std::uint8_t, 3>{ 255, 175, 135 },
        std::array<std::uint8_t, 3>{ 255, 175, 175 },
        std::array<std::uint8_t, 3>{ 255, 175, 215 },
        std::array<std::uint8_t, 3>{ 255, 175, 255 },
        std::array<std::uint8_t, 3>{ 255, 215, 0 },
        std::array<std::uint8_t, 3>{ 255, 215, 95 },
        std::array<std::uint8_t, 3>{ 255, 215, 135 },
        std::array<std::uint8_t, 3>{ 255, 215, 175 },
        std::array<std::uint8_t, 3>{ 255, 215, 215 },
        std::array<std::uint8_t, 3>{ 255, 215, 255 },
        std::array<std::uint8_t, 3>{ 255, 255, 0 },
        std::array<std::uint8_t, 3>{ 255, 255, 95 },
        std::array<std::uint8_t, 3>{ 255, 255, 135 },
        std::array<std::uint8_t, 3>{ 255, 255, 175 },
        std::array<std::uint8_t, 3>{ 255, 255, 215 },
        std::array<std::uint8_t, 3>{ 255, 255, 255 },
        std::array<std::uint8_t, 3>{ 8, 8, 8 },
        std::array<std::uint8_t, 3>{ 18, 18, 18 },
        std::array<std::uint8_t, 3>{ 28, 28, 28 },
        std::array<std::uint8_t, 3>{ 38, 38, 38 },
        std::array<std::uint8_t, 3>{ 48, 48, 48 },
        std::array<std::uint8_t, 3>{ 58, 58, 58 },
        std::array<std::uint8_t, 3>{ 68, 68, 68 },
        std::array<std::uint8_t, 3>{ 78, 78, 78 },
        std::array<std::uint8_t, 3>{ 88, 88, 88 },
        std::array<std::uint8_t, 3>{ 98, 98, 98 },
        std::array<std::uint8_t, 3>{ 108, 108, 108 },
        std::array<std::uint8_t, 3>{ 118, 118, 118 },
        std::array<std::uint8_t, 3>{ 128, 128, 128 },
        std::array<std::uint8_t, 3>{ 138, 138, 138 },
        std::array<std::uint8_t, 3>{ 148, 148, 148 },
        std::array<std::uint8_t, 3>{ 158, 158, 158 },
        std::array<std::uint8_t, 3>{ 168, 168, 168 },
        std::array<std::uint8_t, 3>{ 178, 178, 178 },
        std::array<std::uint8_t, 3>{ 188, 188, 188 },
        std::array<std::uint8_t, 3>{ 198, 198, 198 },
        std::array<std::uint8_t, 3>{ 208, 208, 208 },
        std::array<std::uint8_t, 3>{ 218, 218, 218 },
        std::array<std::uint8_t, 3>{ 228, 228, 228 }
    };
    constexpr size_t color_distance(const std::array<std::uint8_t, 3>& colorA, const std::array<std::uint8_t, 3>& colorB) noexcept
    {
        size_t dist{ 0 };

        for (size_t idx = 0; idx < colorA.size(); ++idx) {
            auto diff = static_cast<int>(colorA[idx]) - static_cast<int>(colorB[idx]);
            dist += diff > 0 ? diff : -diff;
        }
        return dist;
    }
    constexpr size_t find_closest_color(const std::array<std::uint8_t, 3>& color) noexcept
    {
        size_t minIdx = 0;
        size_t colorDistance = std::numeric_limits<size_t>::max();
        for (size_t idx = 0; idx < consoleColorValues.size(); ++idx)
        {
            const auto& consoleColor = consoleColorValues[idx];
            const auto dist = color_distance(consoleColor, color);
            if (dist < colorDistance)
            {
                minIdx = idx;
                colorDistance = dist;
                if (dist == 0)
                    return minIdx;
            }
        }
        return minIdx;
    }
}

export namespace nyan
{
    enum class LoggerType : std::uint32_t
    {
        Verbose = 0b1,
        Info = 0b10,
        Warn = 0b100,
        Error = 0b1000,
        Critical = 0b10000,
    };
    template<
        LoggerType type,
#ifdef LOGVERBOSITY
        std::uint32_t verbosity = LOGVERBOSITY,
#else
        std::uint32_t verbosity = static_cast<std::uint32_t>(LoggerType::Verbose) | static_cast<std::uint32_t>(LoggerType::Info) | static_cast<std::uint32_t>(LoggerType::Warn) | static_cast<std::uint32_t>(LoggerType::Error) | static_cast<std::uint32_t>(LoggerType::Critical),
#endif
        bool enableDebugOutput = true,
        bool enableTimestamps = false
    >
    class Logger {

    public:

        Logger() noexcept
        {
        }
        Logger(const Logger& other) noexcept = default;

        Logger(Logger&& other) noexcept :
            m_newLine(std::exchange(other.m_newLine, false))
        {
        }
        Logger& operator=(const Logger& other) noexcept {
            if (this != std::addressof(other)) {
                m_newLine = other.m_newLine;
            }
            return *this;
        }
        Logger& operator=(Logger&& other) noexcept {
            if (this != std::addressof(other)) {
                std::swap(m_newLine, other.m_newLine);
            }
            return *this;
        }
        ~Logger() noexcept {
            if constexpr (!(verbosity & static_cast<uint32_t>(type)))
                return;
            if (!m_newLine)
                return;
            static constexpr std::string_view newLine = "\n";
            filter([&]() {
                output(newLine);
                });
        }

        Logger& message(const std::string_view message) & noexcept {
            filter([&]() {
                output(message);
                });
            return *this;
        }
        Logger& message(const std::array<uint8_t, 3>& color, const std::string_view message) & noexcept {
            filter([&]() {
                output(std::format("\033[38;5;{}m{}\033[0m", impl::find_closest_color(color), message));
                });
            return *this;
        }
        Logger&& message(const std::string_view message) && noexcept {
            filter([&]() {
                output(message);
                });
            return std::move(*this);
        }
        Logger&& message(const std::array<uint8_t, 3>& color, const std::string_view message) && noexcept {
            filter([&]() {
                output(std::format("\033[38;5;{}m{}\033[0m", impl::find_closest_color(color), message));
                });
            return std::move(*this);
        }
        Logger& location(const std::source_location& location = std::source_location::current()) & noexcept {
            filter([&]() {
                output(std::format("{}:{}:{} {}:", location.file_name(), location.line(), location.column(), location.function_name()));
                });
            return *this;
        }
        Logger&& location(const std::source_location& location = std::source_location::current()) && noexcept {
            filter([&]() {
                output(std::format("{}:{}:{} {}:", location.file_name(), location.line(), location.column(), location.function_name()));
                });
            return std::move(*this);
        }
        template<typename ...Args>
        Logger& format(std::format_string<Args...> fmt, Args&&... args) & noexcept {
            filter([&]() {
                output(std::format(fmt, std::forward<Args>(args)...));
                });
            return *this;
        }
        template<typename ...Args>
        Logger&& format(std::format_string<Args...> fmt, Args&&... args) && noexcept {
            filter([&]() {
                output(std::format(fmt, std::forward<Args>(args)...));
                });
            return std::move(*this);
        }
        template<typename ...Args>
        Logger& format_runtime(std::string_view view, Args&&... args) & noexcept {
            filter([&]() {
                output(std::vformat(view, std::make_format_args(args...)));
                });
            return *this;
        }
        template<typename ...Args>
        Logger&& format_runtime(std::string_view view, Args&&... args) && noexcept {
            filter([&]() {
                output(std::vformat(view, std::make_format_args(args...)));
                });
            return std::move(*this);
        }
        template<typename ...Args>
        Logger& format_color_runtime(const std::array<uint8_t, 3>& color, std::format_string<Args...> view, Args&&... args) & noexcept {
            filter([&]() {
                auto col = impl::find_closest_color(color);
                auto str = std::vformat(view, std::make_format_args(args...));
                output(std::vformat("\033[38;5;{}m{}\033[0m", std::make_format_args(col, str)));
                });
            return *this;
        }
        template<typename ...Args>
        Logger&& format_color_runtime(const std::array<uint8_t, 3>& color, std::string_view view, Args&&... args) && noexcept {
            filter([&]() {
                auto col = impl::find_closest_color(color);
                auto str = std::vformat(view, std::make_format_args(args...));
                output(std::vformat("\033[38;5;{}m{}\033[0m", std::make_format_args(col, str)));
                });
            return std::move(*this);
        }
        template<typename ...Args>
        Logger& format_color(const std::array<uint8_t, 3>& color, std::format_string<Args...> fmt, Args&&... args) & noexcept {
            filter([&]() {
                auto col = impl::find_closest_color(color);
                auto str = std::format(fmt, std::forward<Args>(args)...);
                output(std::format("\033[38;5;{}m{}\033[0m", col, str));
                });
            return *this;
        }
        template<typename ...Args>
        Logger&& format_color(const std::array<uint8_t, 3>& color, std::format_string<Args...> fmt, Args&&... args) && noexcept {
            filter([&]() {
                auto col = impl::find_closest_color(color);
                auto str = std::format(fmt, std::forward<Args>(args)...);
                output(std::format("\033[38;5;{}m{}\033[0m", col, str));
                });
            return std::move(*this);
        }
#if __cpp_lib_stacktrace >= 202011L
        Logger& stacktrace(const std::stacktrace& currentStacktrace = std::stacktrace::current()) & noexcept {
            filter([&]() {
                for (const auto& trace : currentStacktrace)
                    output(std::format("{}\n", trace.description()));
                });
            return *this;
        }
        Logger&& stacktrace(const std::stacktrace& currentStacktrace = std::stacktrace::current()) && noexcept {
            filter([&]() {
                for (const auto& trace : currentStacktrace)
                    output(std::format("{}\n", trace.description()));
                });
            return std::move(*this);
        }
#else
        Logger& stacktrace(const std::source_location& location = std::source_location::current()) & noexcept {
            if constexpr (verbosity & static_cast<uint32_t>(type))
                ignore_exceptions([&]() {
                output(std::format("{}:{}:{} {}:", location.file_name(), location.line(), location.column(), location.function_name()));
                    });
            return *this;
        }
        Logger&& stacktrace(const std::source_location& location = std::source_location::current()) && noexcept {
            if constexpr (verbosity & static_cast<uint32_t>(type))
                ignore_exceptions([&]() {
                output(std::format("{}:{}:{} {}:", location.file_name(), location.line(), location.column(), location.function_name()));
                    });
            return std::move(*this);
        }
#endif
    private:
        void output(const std::string& data) noexcept {
            if (!std::is_constant_evaluated())
                std::fwrite(data.data(), sizeof(std::string::value_type), data.size(), stream());
            //OutputDebugString(data);
#ifdef WIN32
            if (enableDebugOutput)
                OutputDebugStringA(data.data());
#endif
        }
        void output(std::string_view view) noexcept {
            //if (!std::is_constant_evaluated())
            //    std::fwrite(view.data(), sizeof(std::string_view::value_type), view.size(), stream());
            output(std::string(view));
            //TODO outputdebugString for string_view

        }
        [[nodiscard]] auto stream() const noexcept {
            if constexpr (type == LoggerType::Error || type == LoggerType::Critical)
                return stderr;
            else if constexpr (type == LoggerType::Info)
                return stdout;
            else
                return stdout;
        }

        void filter(auto fun) noexcept {
            if constexpr (verbosity & static_cast<uint32_t>(type)) {
                try {
                    fun();
                }
                catch (...) {
                    //Ignore mainly bad_allocs
                }
            }

        }
        bool m_newLine = true;

    };

    class log {
    public:
        [[nodiscard]] static Logger< LoggerType::Verbose> verbose() noexcept
        {
            return {};
        }
        [[nodiscard]] static Logger< LoggerType::Warn> warning() noexcept
        {
            return {};
        }
        [[nodiscard]] static Logger< LoggerType::Info> info() noexcept
        {
            return {};
        }
        [[nodiscard]] static Logger< LoggerType::Error>  error() noexcept
        {
            return {};
        }
        [[nodiscard]] static Logger< LoggerType::Critical>  critical() noexcept
        {
            return {};
        }

        auto static flush_verbose() noexcept {

        }
        auto static flush_warning() noexcept {

        }
        auto static flush_info() noexcept {

        }
        auto static flush_error() noexcept {

        }
        auto static flush_critical() noexcept {

        }
        auto static flush() noexcept {
            flush_verbose();
            flush_warning();
            flush_info();
            flush_error();
            flush_critical();
        }
    };
}