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

#ifdef WIN32
#include <intrin.h>
#elif __linux
#include <signal.h>
#endif

export module NYANAssert:Assert;
import std;
import NYANLog;

namespace impl {
	template<typename T>
	concept Invocable = std::is_invocable_r<bool, T>::value;

	template<typename T>
	concept ConvertibleBool = requires(T a) { static_cast<bool>(a); };

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
		template<impl::ConvertibleBool C>
		constexpr Assertable(C&& condition) noexcept :
			m_condition(static_cast<bool>(condition))
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
			Critical,
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
		enum class AssertionDebugMode {
			Disabled,
			Enabled,
		};
#ifdef ASSERTION_LEVEL
		inline constexpr AssertionLevel assertionLevel = static_cast<AssertionLevel>(ASSERTION_LEVEL);
		static_assert(ASSERTION_LEVEL < 3);
		static_assert(ASSERTION_LEVEL >= 0);
#undef ASSERTION_LEVEL
#else 
#ifdef NDEBUG
		inline constexpr AssertionLevel assertionLevel = AssertionLevel::Disabled;
#else
		inline constexpr AssertionLevel assertionLevel = AssertionLevel::Enabled;
#endif
#endif

#ifdef ASSERTION_EXIT_MODE
		inline constexpr AssertionExitMode assertionExitMode = static_cast<AssertionExitMode>(ASSERTION_EXIT_MODE);
		static_assert(ASSERTION_EXIT_MODE < 4);
		static_assert(ASSERTION_EXIT_MODE >= 0);
#undef ASSERTION_EXIT_MODE
#else 
#ifdef NDEBUG
		inline constexpr AssertionExitMode AssertionExitMode = AssertionExitMode::Disabled;
#else
		inline constexpr AssertionExitMode AssertionExitMode = AssertionExitMode::Abort;
#endif
#endif

#ifdef ASSERTION_LOG_MODE
		inline constexpr AssertionLogMode assertionLoggingBehavior = static_cast<AssertionLogMode>(ASSERTION_LOG_MODE);
		static_assert(ASSERTION_LOG_MODE < 4);
		static_assert(ASSERTION_LOG_MODE >= 0);
#undef ASSERTION_LOG_MODE
#else 
#ifdef NDEBUG
		inline constexpr AssertionLogMode assertionLoggingBehavior = AssertionLogMode::Disabled ;
#else
		inline constexpr AssertionLogMode assertionLoggingBehavior = AssertionLogMode::StackTrace;
#endif
#endif

#ifdef ASSERTION_DEBUG_MODE
		inline constexpr AssertionDebugMode assertionDebugBehavior = static_cast<AssertionDebugMode>(ASSERTION_DEBUG_MODE);
		static_assert(ASSERTION_DEBUG_MODE < 2);
		static_assert(ASSERTION_DEBUG_MODE >= 0);
#undef ASSERTION_DEBUG_MODE
#else 
#ifdef NDEBUG
		inline constexpr AssertionDebugMode assertionDebugBehavior = AssertionDebugMode::Disabled;
#else
		inline constexpr AssertionDebugMode assertionDebugBehavior = AssertionDebugMode::Enabled;
#endif
#endif

		template<AssertionLevel level = assertionLevel, AssertionExitMode exitMode = assertionExitMode, AssertionLogMode logMode = assertionLoggingBehavior, AssertionDebugMode debugBehavior = assertionDebugBehavior>
		struct Assert {


			constexpr void operator()([[maybe_unused]] impl::Assertable<(level != AssertionLevel::Disabled)> condition,
				[[maybe_unused]] std::string_view msg = "",
				[[maybe_unused]] const std::source_location& location = std::source_location::current()) const noexcept
			{
				if constexpr (level == AssertionLevel::Disabled)
					return;
				if (condition)
					return;

				if constexpr (logMode == AssertionLogMode::MessageOnly)
					nyan::log::error().message(msg);
				else if constexpr (logMode == AssertionLogMode::SourceLocation)
					nyan::log::error().message(msg).message("\n").location(location);
				else if constexpr (logMode == AssertionLogMode::StackTrace)
//#ifdef USE_STACKTRACE
					nyan::log::error().message(msg).message("\n").stacktrace(std::stacktrace::current(1));
//#else
					//nyan::log::error().message(msg).message("\n").location(location);
//#endif
				if (std::is_constant_evaluated())
					throw 0;

#ifdef WIN32
				if constexpr (debugBehavior == AssertionDebugMode::Enabled)
					__debugbreak();
#elif __linux
				if constexpr (debugBehavior == AssertionDebugMode::Enabled)
					raise(SIGTRAP);
#endif

				if constexpr (exitMode == AssertionExitMode::QuickExit)
					std::quick_exit(1); 
				else if constexpr (exitMode == AssertionExitMode::Exit) 
					std::exit(1);
				else if constexpr (exitMode == AssertionExitMode::Abort)
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
		template<AssertionExitMode exitMode = assertionExitMode, AssertionLogMode assertionLogMode = assertionLoggingBehavior, AssertionDebugMode debugBehavior = assertionDebugBehavior>
		constexpr auto AssertionsEnabled = Assert< AssertionLevel::Enabled, exitMode, assertionLogMode, debugBehavior>{};

		template<AssertionExitMode exitMode = assertionExitMode, AssertionLogMode assertionLogMode = assertionLoggingBehavior, AssertionDebugMode debugBehavior = assertionDebugBehavior>
		constexpr auto AssertionsDisabled = Assert< AssertionLevel::Disabled, exitMode, assertionLogMode, debugBehavior>{};

		template<AssertionExitMode exitMode = assertionExitMode, AssertionLogMode assertionLogMode = assertionLoggingBehavior, AssertionDebugMode debugBehavior = assertionDebugBehavior>
		constexpr auto AssertionsCritical = Assert< AssertionLevel::Critical, exitMode, assertionLogMode, debugBehavior>{};


		constexpr auto defaultAssert = Assert<assertionLevel, assertionExitMode, assertionLoggingBehavior, assertionDebugBehavior>{};


	};

}