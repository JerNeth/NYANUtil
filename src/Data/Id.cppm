module;

export module NYANData:Id;
import std;

import :ModuleSettings;


export namespace nyan
{
	template<std::unsigned_integral T, class F>
	class Id {
		friend F;
	public:
		constexpr Id() noexcept : m_value(std::numeric_limits<T>::max()) {};
		friend bool operator==(const Id& a, const Id& b) noexcept = default;

		explicit constexpr operator T() const noexcept { return m_value; }
		constexpr static Id invalid() noexcept { return Id{}; }
		constexpr operator bool() const noexcept { return *this == invalid(); }
	private:
		constexpr Id(T value) noexcept {};

		T m_value;
	};
}
