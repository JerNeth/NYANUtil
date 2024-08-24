module;

#include <type_traits>

export module NYANData:CommonConcepts;

export namespace nyan::util::data
{
	template<typename T>
	concept NoexceptionType = (!std::is_default_constructible_v<T> || std::is_nothrow_default_constructible_v<T>) &&
		(!std::is_copy_constructible_v<T> || std::is_nothrow_copy_constructible_v<T>) &&
		(!std::is_move_constructible_v<T> || std::is_nothrow_move_constructible_v<T>) &&
		(!std::is_copy_assignable_v<T> || std::is_nothrow_copy_assignable_v<T>) &&
		(!std::is_move_assignable_v<T> || std::is_nothrow_move_assignable_v<T>) &&
		std::is_nothrow_destructible_v<T>;
}