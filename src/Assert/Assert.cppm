module;

//#if __cpp_lib_expected >= 202202L
//#include <expected>
//#endif
//#include <type_traits>
//#include <source_location>
//#include <string>
//#include <string_view>
//#include <array>
//#include <iostream>
//#include <vector>
//#include <format>
//
//#if __cpp_lib_stacktrace >= 202011L
//#define USE_STACKTRACE
//#endif
//
//#ifdef USE_STACKTRACE
//#include <stacktrace>
//#endif

export module NYANAssert:Assert;
import std;
import NYANLog;

namespace impl {
	template<typename T>
	concept Invocable = std::is_invocable_r<bool, T>::value;

	template <bool EvaluationEnabled>
	class Assertable {
	public:
		template<impl::Invocable Fun>
		constexpr Assertable(Fun&& fun) noexcept
		{
			if constexpr (EvaluationEnabled)
				m_condition = std::invoke(std::forward<Fun>(fun));
			else
				m_condition = true;
		}
		constexpr Assertable(bool a) noexcept :
			m_condition(a)
		{
		}

		constexpr operator bool() const noexcept { return m_condition; }
	private:
		bool m_condition{};
	};
}

export namespace nyan
{
	namespace assert {
		enum class AssertionLevel {
			Disabled,
			PassThrough,
			Enabled
		};
		enum class AssertionExitMode {
			Disabled,
			Exit,
			QuickExit,
			Abort
		};
		enum class AssertionLogMode {
			Disabled,
			MessageOnly,
			SourceLocation,
			StackTrace,
		};
#ifdef ASSERTION_LEVEL
		inline constexpr AssertionLevel assertionLevel = static_cast<AssertionLevel>(ASSERTION_LEVEL);
		static_assert(ASSERTION_LEVEL < 3);
		static_assert(ASSERTION_LEVEL >= 0);
#undef ASSERTION_LEVEL
#else 
#ifdef NDEBUG
		inline constexpr AssertionLevel assertionLevel = AssertionLevel::Enabled;
#else
		inline constexpr AssertionLevel assertionLevel = AssertionLevel::Disabled;
#endif
#endif

#ifdef ASSERTION_EXIT_MODE
		inline constexpr AssertionExitMode assertionExitMode = static_cast<AssertionExitMode>(ASSERTION_EXIT_MODE);
		static_assert(ASSERTION_EXIT_MODE < 4);
		static_assert(ASSERTION_EXIT_MODE >= 0);
#undef ASSERTION_EXIT_MODE
#else 
#ifdef NDEBUG
		inline constexpr AssertionExitMode AssertionExitMode = AssertionExitMode::Abort;
#else
		inline constexpr AssertionExitMode AssertionExitMode = AssertionExitMode::Disabled;
#endif
#endif

#ifdef ASSERTION_LOG_MODE
		inline constexpr AssertionLogMode loggingBehavior = static_cast<AssertionLogMode>(ASSERTION_LOG_MODE);
		static_assert(ASSERTION_LOG_MODE < 4);
		static_assert(ASSERTION_LOG_MODE >= 0);
#undef ASSERTION_LOG_MODE
#else 
#ifdef NDEBUG
		inline constexpr AssertionLogMode loggingBehavior = AssertionLogMode::StackTrace;
#else
		inline constexpr AssertionLogMode loggingBehavior = AssertionLogMode::Disabled;
#endif
#endif

		template<size_t level = static_cast<size_t>(assertionLevel), size_t exitMode = static_cast<size_t>(assertionExitMode), size_t assertionLogMode = static_cast<size_t>(loggingBehavior)>
		struct Assert {


			constexpr void operator()([[maybe_unused]] impl::Assertable<(level != static_cast<size_t>(AssertionLevel::Disabled))> condition,
				[[maybe_unused]] std::string_view msg = "",
				[[maybe_unused]] const std::source_location& location = std::source_location::current()) const noexcept
			{
				if constexpr (level == static_cast<size_t>(AssertionLevel::Disabled))
					return;
				if (condition)
					return;

				if constexpr (assertionLogMode == static_cast<size_t>(AssertionLogMode::MessageOnly))
					nyan::log::error().message(msg);
				else if constexpr (assertionLogMode == static_cast<size_t>(AssertionLogMode::SourceLocation))
					nyan::log::error().message(msg).message("\n").location(location);
				else if constexpr (assertionLogMode == static_cast<size_t>(AssertionLogMode::StackTrace))
//#ifdef USE_STACKTRACE
					nyan::log::error().message(msg).message("\n").stacktrace(std::stacktrace::current(1));
//#else
					//nyan::log::error().message(msg).message("\n").location(location);
//#endif


				if constexpr (exitMode == static_cast<size_t>(AssertionExitMode::QuickExit))
					std::quick_exit(1);
				else if constexpr (exitMode == static_cast<size_t>(AssertionExitMode::Exit))
					std::exit(1);
				else if constexpr (exitMode == static_cast<size_t>(AssertionExitMode::Abort))
					std::abort();
			}

			//constexpr void expects() noexcept
			//{
			//}

			//template<typename T>
			//constexpr T ensures() noexcept
			//{
			//	return T{};
			//}
		};
		//constexpr Assert assert = Assert{};
	};

}