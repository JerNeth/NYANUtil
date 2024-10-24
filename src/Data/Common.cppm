module;

//#include <type_traits>

export module NYANData:Common;
import std;

export namespace nyan
{
	template<typename T>
	concept NoexceptionType = (!std::is_default_constructible_v<T> || std::is_nothrow_default_constructible_v<T>) &&
		(!std::is_copy_constructible_v<T> || std::is_nothrow_copy_constructible_v<T>) &&
		(!std::is_move_constructible_v<T> || std::is_nothrow_move_constructible_v<T>) &&
		(!std::is_copy_assignable_v<T> || std::is_nothrow_copy_assignable_v<T>) &&
		(!std::is_move_assignable_v<T> || std::is_nothrow_move_assignable_v<T>) &&
		std::is_nothrow_destructible_v<T>;

	struct Ignore { 
		template <typename T>
		constexpr const Ignore& operator=(const T&) const noexcept {
			return *this;
		}
	};

	constexpr Ignore ignore{};
}