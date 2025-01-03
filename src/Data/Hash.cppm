module;

//#include <bit>
//#include <cstdint>
//#include <span>
//#include <tuple>

export module NYANData:Hash;
import std;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;

namespace impl
{
	//template<typename T>
	//concept Tupple17 = requires (T a)
	//{
	//	{ auto [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17] = a };
	//};
	//template<typename T>
	//concept Tupple2 = requires (T a)
	//{
	//	{ auto [p1, p2] = a };
	//};
	template<class T, typename... Args>
	decltype(void(T{ std::declval<Args>()... }), std::true_type())
		test(int);

	template<class T, typename... Args>
	std::false_type
		test(...);

	template<class T, typename... Args>
	struct is_braces_constructible : decltype(test<T, Args...>(0))
	{
	};
	struct any_type {
		template<class T>
		constexpr operator T(); // non explicit
	};
	template<class T>
	constexpr auto to_tuple(T&& object) noexcept {
		using type = std::decay_t<T>;
		if constexpr (is_braces_constructible<type, any_type, any_type, any_type, any_type>{}) {
			auto&& [p1, p2, p3, p4, p5, p6] = object;
			return std::make_tuple(p1, p2, p3, p4, p5, p6);
		}
		else if constexpr (is_braces_constructible<type, any_type, any_type, any_type, any_type>{}) {
			auto&& [p1, p2, p3, p4, p5] = object;
			return std::make_tuple(p1, p2, p3, p4, p5);
		}
		else if constexpr (is_braces_constructible<type, any_type, any_type, any_type, any_type>{}) {
			auto&& [p1, p2, p3, p4] = object;
			return std::make_tuple(p1, p2, p3, p4);
		}
		else if constexpr (is_braces_constructible<type, any_type, any_type, any_type>{}) {
			auto&& [p1, p2, p3] = object;
			return std::make_tuple(p1, p2, p3);
		}
		else if constexpr (is_braces_constructible<type, any_type, any_type>{}) {
			auto&& [p1, p2] = object;
			return std::make_tuple(p1, p2);
		}
		else if constexpr (is_braces_constructible<type, any_type>{}) {
			auto&& [p1] = object;
			return std::make_tuple(p1);
		}
		else {
			return std::make_tuple();
		}
	}
	//template<typename T>
	//[[nodiscard]] constexpr auto to_tuple(T&& obj) requires Tupple17<T> 
	//{
	//	auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17] = std::forward<T>(obj);
	//	return std::tuple{ p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17 };
	//}
	//template<typename T>
	//[[nodiscard]] constexpr auto to_tuple(T&& obj) requires Tupple2<T>
	//{
	//	auto&& [p1, p2] = std::forward<T>(obj);
	//	return std::tuple{ p1, p2 };
	//}
	//template<typename T>
	//[[nodiscard]] constexpr auto to_tuple(T&& obj)
	//{
	//	if constexpr (requires{[&obj] {auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17] = obj; }; })
	//	{
	//		auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17] = std::forward<T>(obj);
	//		return std::tuple{ p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17 };
	//	}
	//	else if constexpr (requires{[&obj] {auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16] = obj; }; })
	//	{
	//		auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16] = std::forward<T>(obj);
	//		return std::tuple{ p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16 };
	//	}
	//	else if constexpr (requires{[&obj] {auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15] = obj; }; })
	//	{
	//		auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15] = std::forward<T>(obj);
	//		return std::tuple{ p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15 };
	//	}
	//	else if constexpr (requires{[&obj] {auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14] = obj; }; })
	//	{
	//		auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14] = std::forward<T>(obj);
	//		return std::tuple{ p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14 };
	//	}
	//	else if constexpr (requires{[&obj] {auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13] = obj; }; })
	//	{
	//		auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13] = std::forward<T>(obj);
	//		return std::tuple{ p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13 };
	//	}
	//	else if constexpr (requires{[&obj] {auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12] = obj; }; })
	//	{
	//		auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12] = std::forward<T>(obj);
	//		return std::tuple{ p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12 };
	//	}
	//	else if constexpr (requires{[&obj] {auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11] = obj; }; })
	//	{
	//		auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11] = std::forward<T>(obj);
	//		return std::tuple{ p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11 };
	//	}
	//	else if constexpr (requires{[&obj] {auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10] = obj; }; })
	//	{
	//		auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10] = std::forward<T>(obj);
	//		return std::tuple{ p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 };
	//	}
	//	else if constexpr (requires{[&obj] {auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9] = obj; }; })
	//	{
	//		auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9] = std::forward<T>(obj);
	//		return std::tuple{ p1, p2, p3, p4, p5, p6, p7, p8, p9 };
	//	}
	//	else if constexpr (requires{[&obj] {auto&& [p1, p2, p3, p4, p5, p6, p7, p8] = obj; }; })
	//	{
	//		auto&& [p1, p2, p3, p4, p5, p6, p7, p8] = std::forward<T>(obj);
	//		return std::tuple{ p1, p2, p3, p4, p5, p6, p7, p8 };
	//	}
	//	else if constexpr (requires{[&obj] {auto&& [p1, p2, p3, p4, p5, p6, p7] = obj; }; })
	//	{
	//		auto&& [p1, p2, p3, p4, p5, p6, p7] = std::forward<T>(obj);
	//		return std::tuple{ p1, p2, p3, p4, p5, p6, p7 };
	//	}
	//	else if constexpr (requires{[&obj] {auto&& [p1, p2, p3, p4, p5, p6] = obj; }; })
	//	{
	//		auto&& [p1, p2, p3, p4, p5, p6] = std::forward<T>(obj);
	//		return std::tuple{ p1, p2, p3, p4, p5, p6 };
	//	}
	//	else if constexpr (requires{[&obj] {auto&& [p1, p2, p3, p4, p5] = obj; }; })
	//	{
	//		auto&& [p1, p2, p3, p4, p5] = std::forward<T>(obj);
	//		return std::tuple{ p1, p2, p3, p4, p5 };
	//	}
	//	else if constexpr (requires{[&obj] {auto&& [p1, p2, p3, p4] = obj; }; })
	//	{
	//		auto&& [p1, p2, p3, p4] = std::forward<T>(obj);
	//		return std::tuple{ p1, p2, p3, p4 };
	//	}
	//	else if constexpr (requires{[&obj] {auto&& [p1, p2, p3] = obj; }; })
	//	{
	//		auto&& [p1, p2, p3] = std::forward<T>(obj);
	//		return std::tuple{ p1, p2, p3 };
	//	}
	//	else if constexpr (requires{[&obj] {auto&& [p1, p2] = obj; }; })
	//	{
	//		auto&& [p1, p2] = std::forward<T>(obj);
	//		return std::tuple{ p1, p2 };
	//	}
	//	else if constexpr (requires {[&obj] { auto&& [p1] = obj; }; })
	//	{
	//		auto&& [p1] = std::forward<T>(obj);
	//		return std::tuple{ p1 };
	//	}
	//	else
	//	{
	//		return std::tuple{};
	//	}

	//}
}

export namespace nyan
{
	using HashValue = std::uint64_t;


	template<typename T>
	concept Fundamental = std::is_fundamental_v<T> && (sizeof(T) == 8 || sizeof(T) == 4 || sizeof(T) == 2 || sizeof(T) == 1) ;

	template<Fundamental T>
	[[nodiscard]] constexpr HashValue hash(const T& t) noexcept
	{

		constexpr HashValue prime{ 0x100000001b3ull };
		HashValue hashVal = 0xcbf29ce484222325ull;
		if constexpr (sizeof(T) == 8)
			return (hashVal ^ std::bit_cast<uint64_t>(t)) * prime;
		else if constexpr (sizeof(T) == 4)
			return (hashVal ^ std::bit_cast<uint32_t>(t)) * prime;
		else if constexpr (sizeof(T) == 2)
			return (hashVal ^ std::bit_cast<uint16_t>(t)) * prime;
		else if constexpr (sizeof(T) == 1)
			return (hashVal ^ std::bit_cast<uint8_t>(t)) * prime;

		return hashVal;
	}
	template <typename C> struct is_span : std::false_type {};
	template <typename T> struct is_span< std::span<T> > : std::true_type {};
	template <typename C> inline constexpr bool is_span_v = is_span<C>::value;

	template<typename T>
	concept Trivial = std::is_trivially_copyable_v<T> && !Fundamental<T> && !is_span_v<T>;



	template<Trivial T>
	[[nodiscard]] constexpr HashValue hash(const T& t) noexcept
	{

		constexpr HashValue prime{ 0x100000001b3ull };
		HashValue hashVal = 0xcbf29ce484222325ull;

		auto fun = [&](const auto& val)
			{
				hashVal = (hashVal ^ hash(val)) * prime;
				return hashVal;
			};
		
		std::apply([&hashVal, &fun](auto&&... args) { (fun(args), ...); }, impl::to_tuple(t));

		return hashVal;
	}

	template<typename T>
	concept ByteHashable = std::is_same_v<T, std::byte> || std::is_same_v<T, unsigned char>;

	template<typename T>
	concept Hashable = !ByteHashable<T> && requires(T a)
	{
		{ hash<T>(a) } -> std::convertible_to<HashValue>;
	};


	template<Hashable T, size_t Extent>
	[[nodiscard]] constexpr HashValue hash(std::span<T, Extent> data) noexcept
	{
		static constexpr HashValue prime{ 0x100000001b3ull };
		HashValue hash = 0xcbf29ce484222325ull;
		for (const auto& date : data)
			hash = (hash ^ nyan::hash(date)) * prime;
		return hash;
	}


	template<ByteHashable T, size_t Extent>
	[[nodiscard]] constexpr HashValue hash(std::span<T, Extent> data) noexcept
	{
		static constexpr HashValue prime{ 0x100000001b3ull };
		HashValue hash = 0xcbf29ce484222325ull;

		size_t idx = 0;

		for(; idx < data.size(); idx += sizeof(HashValue))
			hash = (hash ^ *reinterpret_cast<HashValue*>(data.data() + idx)) * prime;

		for (; idx < data.size(); idx ++)
			hash = (hash ^ *reinterpret_cast<unsigned char*>(data.data() + idx)) * prime;

		return hash;
	}
}
