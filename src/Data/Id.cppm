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

		constexpr static Id invalid() noexcept { return Id{}; }

	private:
		constexpr Id(T value) noexcept {};

		T m_value;
	};
}
