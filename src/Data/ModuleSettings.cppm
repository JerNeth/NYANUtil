module;

export module NYANData:ModuleSettings;
import NYANAssert;

#ifdef NDEBUG
export constexpr inline auto assert = nyan::assert::AssertionsDisabled<nyan::assert::assertionExitMode, nyan::assert::loggingBehavior>;
#else
export constexpr inline auto assert = nyan::assert::AssertionsEnabled<nyan::assert::assertionExitMode, nyan::assert::loggingBehavior>;
#endif