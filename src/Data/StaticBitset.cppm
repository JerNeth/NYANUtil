module;

//#include <array>
//#include <bit>
//#include <limits>
//#include <iostream>
//#include <string_view>

export module NYANData:StaticBitset;
import std;

import NYANAssert;

import :ModuleSettings;

using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
namespace impl {
	template<class F, class T>
	concept Callable = requires(F && f, T && t) { f(t); };
}
export namespace nyan
{
	template<size_t bitSize, typename T = size_t> // typename for indices e.g. enums
	class bitset {
		using bitType = std::conditional_t< bitSize <= 8, uint8_t, std::conditional_t < bitSize <= 16, uint16_t, std::conditional_t < bitSize <= 32, uint32_t, uint64_t>>>;
		static constexpr size_t bitsPerWord = (sizeof(bitType) * 8);
		static constexpr size_t bitsPerWordBitPos = std::countr_zero(bitsPerWord);
		static_assert(std::has_single_bit(bitsPerWord));
		static constexpr size_t bitsMask = bitsPerWord - 1;
		static constexpr size_t typeSize = bitSize / bitsPerWord + (bitSize % bitsPerWord != 0);
		static_assert(bitSize <= (std::numeric_limits <std::conditional_t< std::is_enum_v<T>, std::underlying_type_t<T>, T>>::max()));
		//static_assert(std::is_convertible<T, size_t>::value);
	public:
		constexpr bitset() noexcept : m_data() {
			for(auto& data : m_data)
				data = 0;
		}
		constexpr bitset(bitType t) noexcept {
			m_data[0] = t;
		}
		template<class Head, class... Tail>
		using are_same = std::conjunction<std::is_same<Head, Tail>...>;
		template<typename... Tail, class = std::enable_if_t<are_same<T, Tail...>::value, void>>
		constexpr bitset(Tail... args) noexcept {
			*this = (*this | ... | args);
		}
		[[nodiscard]] constexpr bool test(T _idx) const noexcept {
			const size_t idx = static_cast<size_t>(_idx);
			::assert(idx < bitSize);
			auto& word = m_data[idx >> bitsPerWordBitPos];
			const auto bit = 1ull << (idx & bitsMask);
			return static_cast<decltype(bit)>(word) & bit;
		}
		[[nodiscard]] constexpr bool only(T _idx) const noexcept {
			const size_t idx = static_cast<size_t>(_idx);
			::assert(idx < bitSize);
			if constexpr (typeSize > 1) {
				for (size_t i = 0; i < (idx >> bitsPerWordBitPos); i++) {
					if (m_data[i] != 0)
						return false;
				}
			}
			auto& word = m_data[idx >> bitsPerWordBitPos];
			const auto bit = 1ull << (idx & bitsMask);
			if (static_cast<decltype(bit)>(word) != bit)
				return false;

			if constexpr (typeSize > 1) {
				for (size_t i = (idx >> bitsPerWordBitPos) + 1; i < typeSize; i++) {
					if (m_data[i] != 0)
						return false;
				}
			}
			return true;
		}
		template<class Head, class... Tail>
		using are_same = std::conjunction<std::is_same<Head, Tail>...>;
		template<typename... Tail, class = std::enable_if_t<are_same<T, Tail...>::value, void>>
		constexpr bool any_of(Tail... args) const noexcept {
			bitset flags;
			flags = (flags | ... | args);
			for (size_t i = 0; i < typeSize; i++) {
				bitType tmp = flags.m_data[i] & m_data[i];
				if (tmp != 0)
					return true;
			}
			return false;
		}
		template<class Head, class... Tail>
		using are_same = std::conjunction<std::is_same<Head, Tail>...>;
		template<typename... Tail, class = std::enable_if_t<are_same<T, Tail...>::value, void>>
		constexpr bool all_of(Tail... args) const noexcept {
			bitset flags;
			flags = (flags | ... | args);
			for (size_t i = 0; i < typeSize; i++) {
				bitType tmp = flags.m_data[i] & m_data[i];
				if (tmp != flags.m_data[i])
					return false;
			}
			return true;
		}

		constexpr bool all_of(bitset other) const noexcept {
			bool all{ true };
			for (size_t i = 0; i < typeSize; i++) {
				all &= (other.m_data[i] & m_data[i]) != other.m_data[i];
			}
			return all;
		}

		template<typename... Tail, class = std::enable_if_t<are_same<T, Tail...>::value, void>>
		constexpr bitset get_and_clear(Tail... args) noexcept {
			bitset flags;
			flags = (flags | ... | args);
			for (size_t i = 0; i < typeSize; i++) {
				bitType tmp = m_data[i];
				m_data[i] &= ~(flags.m_data[i]);
				flags.m_data[i] &= tmp;
			}
			return flags;
		}
		template<class Fun>
		requires impl::Callable< Fun, T>
		constexpr void for_each(Fun&& fun) const noexcept {
			for (size_t i = 0; i < typeSize; i++) {
				auto bits = m_data[i];
				auto offset = bitsPerWord * i;
				for (auto idx = std::countr_zero(bits); bits; bits &= (bits - 1)) { //turns of rightmost bit (Hacker's Delight)
					idx = std::countr_zero(bits);
					fun(static_cast<T>(idx + offset));
				}
			}
		}
		constexpr void fill(std::span<T> dst) const noexcept {
			::assert(dst.size() <= popcount());
			size_t count{ 0 };
			for_each([&dst, &count](T t) {
				dst[count++] = t;
			});

			//constexpr void fill(std::span<std::uint32_t> dst) const noexcept {
			//	assert(dst.size() <= size());
			//	auto bitmaskCopy = bitmask;
			//	size_t count = 0;
			//	while (bitmaskCopy && count < dst.size()) {
			//		std::uint32_t idx = std::countr_zero(bitmaskCopy);
			//		bitmaskCopy ^= 1ull << idx;
			//		dst[count++] = idx;
			//	}
			//}
		}
		constexpr bitset& set() noexcept {
			for (size_t i = 0; i < typeSize; i++) {
				m_data[i] = static_cast<bitType>(~bitType{ 0 });
			}
			return *this;
		}
		constexpr bitset& set(T _idx) noexcept {
			const size_t idx = static_cast<size_t>(_idx);
			::assert(idx < bitSize);
			auto& word = m_data[idx >> bitsPerWordBitPos];
			const auto bit = 1ull << (idx & bitsMask);
			word |= static_cast<bitType>(bit);
			return *this;
		}
		constexpr bitset& set(T _idx, bool value) noexcept {
			const size_t idx = static_cast<size_t>(_idx);
			::assert(idx < bitSize);
			auto& word = m_data[idx >> bitsPerWordBitPos];
			const auto bit = 1ull << (idx & bitsMask);
			if (value)
				word |= static_cast<bitType>(bit);
			else
				word &= ~bit;
			return *this;
		}
		constexpr bitset& reset() noexcept {
			for (size_t i = 0; i < typeSize; i++) {
				m_data[i] = 0u;
			}
			return *this;
		}
		constexpr bitset& reset(T _idx) noexcept {
			const size_t idx = static_cast<size_t>(_idx);
			::assert(idx < bitSize);
			auto& word = m_data[idx >> bitsPerWordBitPos];
			const auto bit = 1ull << (idx & bitsMask);
			word &= ~bit;
			return *this;
		}
		[[nodiscard]] constexpr operator bool() const noexcept {
			return any();
		}
		[[nodiscard]] constexpr size_t popcount() const noexcept {
			size_t ret = 0;
			for (size_t i = 0; i < typeSize; i++) {
				ret += std::popcount(m_data[i]);
			}
			return ret;
		}
		[[nodiscard]] constexpr unsigned long long to_ullong() const noexcept {
			if constexpr (typeSize == 0)
				return 0;
			else {
				return m_data[0];
			}
		}
		[[nodiscard]] constexpr unsigned long to_ulong() const noexcept {
			if constexpr (typeSize == 0)
				return 0;
			else {
				return m_data[0];
			}
		}
		[[nodiscard]] static constexpr size_t size() noexcept {
			return bitSize;
		}
		[[nodiscard]] constexpr bool any() const noexcept {
			for (size_t i = 0; i < typeSize; i++) {
				if (m_data[i] != 0)
					return true;
			}
			return false;
		}
		[[nodiscard]] constexpr bool none() const noexcept {
			for (size_t i = 0; i < typeSize; i++) {
				if (m_data[i] != 0)
					return false;
			}
			return true;
		}
		[[nodiscard]] constexpr bool all() const noexcept {
			if constexpr (bitSize == 0)
				return true;

			static constexpr bool no_padding = ((bitSize & bitsMask) == 0);
			for (size_t i = 0; i < typeSize - !no_padding; i++) {
				if (m_data[i] != ~bitType{ 0u })
					return false;
			}
			return no_padding || m_data[typeSize - 1] == bitsMask;
		}
		constexpr bitset& flip() noexcept {
			for (size_t i = 0; i < typeSize; i++) {
				m_data[i] = ~m_data[i];
			}
			return *this;
		}
		constexpr bitset& flip(T _idx) noexcept {
			const size_t idx = static_cast<size_t>(_idx);
			::assert(idx < bitSize);
			auto& word = m_data[idx >> bitsPerWordBitPos];
			const auto bit = 1ull << (idx & bitsMask);
			word ^= static_cast<bitType>(bit);
			return *this;
		}
		constexpr bitset operator~() const noexcept {
			return bitset(*this).flip();
		}
		constexpr bitset& operator^=(const bitset& rhs) noexcept {
			for (size_t i = 0; i < typeSize; i++) {
				m_data[i] ^= rhs.m_data[i];
			}
			return *this;
		}
		constexpr bitset& operator&=(const bitset& rhs)  noexcept {
			for (size_t i = 0; i < typeSize; i++) {
				m_data[i] &= rhs.m_data[i];
			}
			return *this;
		}
		constexpr bitset& operator|=(const bitset& rhs) noexcept {
			for (size_t i = 0; i < typeSize; i++) {
				m_data[i] |= rhs.m_data[i];
			}
			return *this;
		}
		constexpr bool operator==(const bitset& rhs) const noexcept {
			if constexpr (bitSize == 0)
				return true;

			bool result = true;
			for (size_t i = 0; i < typeSize; i++) {
				result &= m_data[i] == rhs.m_data[i];
			}
			return result;
		}
		constexpr bitset<bitSize, T> operator!=(const bitset<bitSize, T>& rhs) const noexcept {
			return !(*this == rhs);
		}

		constexpr bitset<bitSize, T> operator^(const bitset<bitSize, T>& rhs) const noexcept {
			bitset<bitSize, T> ret = *this;
			return ret ^= rhs;
		}

		constexpr bitset<bitSize, T> operator&(const bitset<bitSize, T>& rhs) const noexcept {
			bitset<bitSize, T> ret = *this;
			return ret &= rhs;
		}

		constexpr bitset<bitSize, T> operator|(const bitset<bitSize, T>& rhs) const noexcept {
			bitset<bitSize, T> ret = *this;
			return ret |= rhs;
		}

		constexpr bitset<bitSize, T> operator|(const T& rhs) const noexcept {
			bitset<bitSize, T> ret;
			ret.set(rhs);
			return ret |= *this;
		}
	private:
		std::array<bitType, typeSize> m_data{};
	};
}