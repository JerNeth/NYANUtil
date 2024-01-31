module;

#include <bit>

export module NYANMath:Half;

import :Util;
import :Constants;

export namespace nyan::util::math
{
	struct half {
	private:
		static constexpr uint32_t floatMantissaBits = 23u;
		static constexpr uint32_t floatExpBits = 8u;
		static constexpr uint32_t floatBits = floatMantissaBits + floatExpBits + 1;
		static constexpr uint32_t floatSignBits = 31u;
		static constexpr uint32_t floatSignBit = 1u << floatSignBits;
		static constexpr uint32_t floatExpMax = (1u << floatExpBits) - 1u;
		static constexpr uint32_t floatExpBias = floatExpMax >> 1;
		static constexpr uint32_t floatInf = floatExpMax << floatMantissaBits;
		static constexpr uint32_t halfMantissaBits = 10u;
		static constexpr uint32_t halfExpBits = 5u;
		static constexpr uint32_t halfBits = halfMantissaBits + halfExpBits + 1;
		static constexpr uint32_t halfSignBits = 15u;
		static constexpr uint32_t halfSignBit = 1 << halfSignBits;
		static constexpr uint32_t halfExpMax = (1 << halfExpBits) - 1;
		static constexpr uint32_t halfExpBias = halfExpMax >> 1;
		static constexpr uint32_t halfInf = halfExpMax << halfMantissaBits;
		static constexpr uint32_t halfQnan = (halfInf | (halfInf >> 1));
		static constexpr uint32_t mantissaDiff = (floatMantissaBits - halfMantissaBits);
		static constexpr uint32_t bitDiff = (floatBits - halfBits);
	public:
		constexpr half() : data(0) {
		}
		constexpr half(float f) {
			constexpr auto bias = std::bit_cast<float>(halfExpBias << floatMantissaBits);
			f *= bias;
			auto bits = std::bit_cast<uint32_t>(f);
			auto sign = bits & floatSignBit;
			bits ^= sign;
			auto isNan = static_cast<uint32_t>(-static_cast<int32_t>(floatInf < bits));
			bits >>= mantissaDiff;
			bits ^= static_cast<uint32_t>(-static_cast<int32_t>(halfInf < bits)) & (halfInf ^ bits);
			bits ^= isNan & (halfQnan ^ bits);
			bits |= sign >> bitDiff;
			data = static_cast<uint16_t>(bits);
		}

		//constexpr explicit half(double d) {
		//	float f = static_cast<float> (d);
		//	constexpr auto bias = std::bit_cast<float>(halfExpBias << floatMantissaBits);
		//	f *= bias;
		//	auto bits = std::bit_cast<uint32_t>(f);
		//	auto sign = bits & floatSignBit;
		//	bits ^= sign;
		//	auto isNan = static_cast<uint32_t>(-static_cast<int32_t>(floatInf < bits));
		//	bits >>= mantissaDiff;
		//	bits ^= static_cast<uint32_t>(-static_cast<int32_t>(halfInf < bits)) & (halfInf ^ bits);
		//	bits ^= isNan & (halfQnan ^ bits);
		//	bits |= sign >> bitDiff;
		//	data = static_cast<uint16_t>(bits);
		//}

		operator float() const {
			constexpr auto bias = std::bit_cast<float>((2u * floatExpBias - halfExpBias) << floatMantissaBits);
			uint32_t bits = data;
			auto sign = bits & halfSignBit;
			bits ^= sign;
			auto isNorm = bits < halfInf;
			bits = (sign << bitDiff) | (bits << mantissaDiff);
			auto val = std::bit_cast<float>(bits) * bias;
			bits = std::bit_cast<uint32_t>(val);
			bits |= -!isNorm & floatInf;
			return std::bit_cast<float>(bits);
		}
		//operator double() const {
		//	constexpr auto bias = std::bit_cast<float>((2u * floatExpBias - halfExpBias) << floatMantissaBits);
		//	uint32_t bits = data;
		//	auto sign = bits & halfSignBit;
		//	bits ^= sign;
		//	auto isNorm = bits < halfInf;
		//	bits = (sign << bitDiff) | (bits << mantissaDiff);
		//	auto val = std::bit_cast<float>(bits) * bias;
		//	bits = std::bit_cast<uint32_t>(val);
		//	bits |= -!isNorm & floatInf;
		//	return static_cast<double>(std::bit_cast<float>(bits));
		//}
		uint16_t data;
	};
}