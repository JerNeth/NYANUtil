module;

#include <type_traits>
#include <cmath>
#include <algorithm>

export module NYANMath:Util;

export namespace Math
{
	struct half;
	template<typename T>
	concept Unsigned = std::is_unsigned_v<T>;
	template<typename T>
	concept Signed = std::is_signed_v<T>;
	template<Unsigned T>
	struct unorm;
	template<Signed T>
	struct snorm;

	template<typename T>
	concept ScalarT = std::is_arithmetic_v<T> || std::is_same_v<T, Math::half> || std::is_same_v<T, Math::unorm<uint8_t>> || std::is_same_v<T, Math::unorm<uint16_t>> || std::is_same_v<T, Math::unorm<uint32_t>> || std::is_same_v<T, Math::unorm<uint64_t>>
		|| std::is_same_v<T, Math::snorm<int8_t>> || std::is_same_v<T, Math::snorm<int16_t>> || std::is_same_v<T, Math::snorm<int32_t>> || std::is_same_v<T, Math::snorm<int64_t>>;

	template<ScalarT S, ScalarT B>
	constexpr auto min(const S& a, const B& b) noexcept -> decltype(a + b) {
		//Assuming IEEE-754
		//If either is NaN, > returns FALSE => min(a,b) gives you the opposite result compared to max(a,b)
		if (!(static_cast<decltype(a + b)>(a) > static_cast<decltype(a + b)>(b)))
			return static_cast<decltype(a + b)>(a);
		return static_cast<decltype(a + b)>(b);
	}
	template<ScalarT S, ScalarT B>
	constexpr auto max(const S& a, const B& b) noexcept-> decltype(a + b) {
		//Assuming IEEE-754
		//If either is NaN, > returns FALSE
		if (static_cast<decltype(a + b)>(a) > static_cast<decltype(a + b)>(b))
			return static_cast<decltype(a + b)>(a);
		return static_cast<decltype(a + b)>(b);
	}
	/*
	If val is NaN => returns min
	*/
	template<ScalarT S>
	constexpr S clamp(const S& val, const S& min, const S& max) noexcept {
		return Math::max(Math::min(val, max), min);
	}
	template<ScalarT S>
	constexpr bool close(const S& a, const S& b, const S& eps = S(1e-5)) noexcept {
		return ((a - eps) <= b) && ((a + eps) >= b);
	}
	template<ScalarT T>
	constexpr const T square(const T& a) noexcept {
		return a * a;
	}
	template<int size, bool column_major = true>
	constexpr int at(int x, int y) {
		if constexpr (column_major) {
			return x * size + y;
		}
		else {
			return y * size + x;
		}
	}
	template<size_t size, bool column_major = true>
	constexpr size_t at(size_t x, size_t y) {
		if constexpr (column_major) {
			return x * size + y;
		}
		else {
			return y * size + x;
		}
	}
	//This function does not handle limits well
	//This function is also stable
	template<typename T, ScalarT U>
	constexpr T lerp(T a, T b, U t) {
		assert(t >= 0.0f && t <= 1.0f);
		//return (1 - t) * a + t * b;
		return a + t * (b - a);
	}

	template<Unsigned T>
	struct unorm {
	private:
	public:
		constexpr unorm() : data(0) {
		}
		constexpr unorm(float f) {
			static constexpr auto max = static_cast<float>(std::numeric_limits<T>::max());
			data = std::clamp(static_cast<T>(std::round(f * max)), std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
		}
		operator float() const {
			static constexpr auto max = 1.f / static_cast<float>(std::numeric_limits<T>::max());
			return static_cast<float>(data) * max;
		}
		T data;
	};
}
