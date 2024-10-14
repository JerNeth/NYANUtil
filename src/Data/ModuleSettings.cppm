module;

export module NYANData:ModuleSettings;
import NYANAssert;

#ifdef NDEBUG
export constexpr inline auto assert = nyan::assert::Assert< static_cast<size_t>(nyan::assert::AssertionLevel::Disabled), static_cast<size_t>(nyan::assert::AssertionExitMode::Disabled), static_cast<size_t>(nyan::assert::AssertionLogMode::Disabled )> {};
#else
export constexpr inline auto assert = nyan::assert::Assert < static_cast<size_t>(nyan::assert::AssertionLevel::Enabled), static_cast<size_t>(nyan::assert::AssertionExitMode::Abort), static_cast<size_t>(nyan::assert::AssertionLogMode::StackTrace)>{};
#endif