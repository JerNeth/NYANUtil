module;

#include <string_view>
#include <source_location>
#include <format>
#include <iostream>

export module NYANLog;

export namespace nyan::util::log
{
	enum class LoggerType : uint32_t
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
		uint32_t verbosity = LOGVERBOSITY
#else
		uint32_t verbosity = static_cast<uint32_t>(LoggerType::Verbose) | static_cast<uint32_t>(LoggerType::Info) | static_cast<uint32_t>(LoggerType::Warn) | static_cast<uint32_t>(LoggerType::Error) | static_cast<uint32_t>(LoggerType::Critical)
#endif
		>
	class Logger {

	public:

		constexpr Logger() noexcept
		{
		}
		constexpr Logger(const Logger& other) noexcept = default;

		constexpr Logger(Logger&& other) noexcept :
			m_newLine(std::exchange(other.m_newLine, false))
		{
		}
		constexpr Logger& operator=(const Logger& other) noexcept {
			if (this != std::addressof(other)) {
				m_newLine = other.m_newLine;
			}
			return *this;
		}
		constexpr Logger& operator=(Logger&& other) noexcept {
			if (this != std::addressof(other)) {
				m_newLine = other.m_newLine;
				other.m_newLine = false;
			}
			return *this;
		}
		~Logger() {
			if constexpr (!(verbosity & static_cast<uint32_t>(type)))
				return;
			if (!m_newLine)
				return;
			stream() << '\n';
		}
		Logger& message(const std::string_view message) & {
			if constexpr (verbosity & static_cast<uint32_t>(type))
				stream() << message;
			return *this;
		}
		Logger&& message(const std::string_view message) && {
			if constexpr (verbosity & static_cast<uint32_t>(type))
				stream() << message;
			return std::move(*this);
		}
		Logger& location(const std::source_location location = std::source_location::current()) & {
			if constexpr (verbosity & static_cast<uint32_t>(type))
				stream() << "file: "
				<< location.file_name() << "("
				<< location.line() << ":"
				<< location.column() << ") `"
				<< location.function_name() << "`: ";
			//OutputDebugString(std::vformat(view, std::make_format_args(args...)));
			return *this;
		}
		Logger&& location(const std::source_location location = std::source_location::current())&& {
			if constexpr (verbosity & static_cast<uint32_t>(type))
				stream() << "file: "
				<< location.file_name() << "("
				<< location.line() << ":"
				<< location.column() << ") `"
				<< location.function_name() << "`: ";
			//OutputDebugString(std::vformat(view, std::make_format_args(args...)));
			return std::move(*this);
		}
		template<typename ...Args>
		Logger& format(std::string_view view, Args&&... args) & {
			if constexpr (verbosity & static_cast<uint32_t>(type))
				stream() << std::vformat(view, std::make_format_args(args...));
			//OutputDebugString(std::vformat(view, std::make_format_args(args...)));
			return *this;
		}
		template<typename ...Args>
		Logger&& format(std::string_view view, Args&&... args)&& {
			if constexpr (verbosity & static_cast<uint32_t>(type))
				stream() << std::vformat(view, std::make_format_args(args...));
			//OutputDebugString(std::vformat(view, std::make_format_args(args...)));
			return std::move(*this);
		}
	private:
		[[nodiscard]] std::ostream& stream() const noexcept {
			if constexpr (type == LoggerType::Error) {
				return std::cerr;
			}
			if constexpr (type == LoggerType::Info) {
				return std::cout;
			}
			return std::cout;
		}
		bool m_newLine = true;
	};
	
	[[nodiscard]] Logger< LoggerType::Verbose> verbose() noexcept
	{
		return {};
	}
	[[nodiscard]] Logger< LoggerType::Warn> warning() noexcept
	{
		return {};
	}
	[[nodiscard]] Logger< LoggerType::Info> info() noexcept
	{
		return {};
	}
	[[nodiscard]] Logger< LoggerType::Error>  error() noexcept
	{
		return {};
	}
	[[nodiscard]] Logger< LoggerType::Critical>  critical() noexcept
	{
		return {};
	}

	auto verbose_message(const std::string_view message)
	{
		return verbose().message(message);
	}
	auto warning_message(const std::string_view message)
	{
		return warning().message(message);
	}
	auto info_message(const std::string_view message)
	{
		return info().message(message);
	}
	auto error_message(const std::string_view message)
	{
		return error().message(message);
	}
	auto critical_message(const std::string_view message)
	{
		return critical().message(message);
	}
}