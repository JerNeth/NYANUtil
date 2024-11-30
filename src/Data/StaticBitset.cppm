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

	template<typename T>
	struct identity { using type = T; };

	template<typename T>
	struct underlying_type { using type = std::underlying_type_t<T>; };

	template<typename T>
	struct robust_underlying { using type = std::conditional_t< std::is_enum_v<T>, impl::underlying_type<T>, impl::identity<T>>::type; };
	
	template<typename T>
	using robust_underlying_t = robust_underlying<T>::type;

	template<size_t bitSize>
	struct smallest_fitting_uint { using type = std::conditional_t< bitSize <= 8, uint8_t, std::conditional_t < bitSize <= 16, uint16_t, std::conditional_t < bitSize <= 32, uint32_t, uint64_t>>>;  };


	template<class Head, class... Tail>
	using are_same = std::conjunction<std::is_same<Head, Tail>...>;

}

export namespace nyan
{
	template<size_t bitSize, typename T = size_t> // typename for indices e.g. enums
	class bitset {
		using bitType = impl::smallest_fitting_uint<bitSize>::type;
		static constexpr size_t bitsPerWord = (sizeof(bitType) * 8);
		static constexpr size_t bitsPerWordBitPos = std::countr_zero(bitsPerWord);
		static_assert(std::has_single_bit(bitsPerWord));
		static constexpr size_t bitsMask = bitsPerWord - 1;
		static constexpr size_t typeSize = bitSize / bitsPerWord + (bitSize % bitsPerWord != 0);
		static constexpr size_t lastWordBitSize = bitSize - ((typeSize - 1) * bitsPerWord);
		static constexpr size_t tailBitsMask = (1ull << lastWordBitSize) - 1;

		static_assert(typeSize > 0);
		static_assert(bitSize <= std::numeric_limits<impl::robust_underlying_t<T>>::max());
		//static_assert(std::is_convertible<T, size_t>::value);
	public:

		struct Iterator
		{
			//using iterator_category = std::iter;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using const_pointer = const value_type*;  // or also value_type*
			using const_reference = const value_type&;  // or also value_type&

			value_type operator*() const noexcept 
			{ 
				return static_cast<value_type>(m_bitIdx + m_idx * bitsPerWord);
			}

			Iterator& operator++() noexcept 
			{
				auto mask = ~((((1ull << m_bitIdx) - 1ull) << 1ull) | 1ull);
				//auto bits = m_ptr[m_idx];
				//bits &= mask;
				//for (auto idx = std::countr_zero(bits); bits; bits &= (bits - 1)) { //turns of rightmost bit (Hacker's Delight)
				//	idx = std::countr_zero(bits);
				//	m_bitIdx = idx;
				//	return *this;
				//}
				//m_idx++;
				for (; m_idx < (typeSize - 1); m_idx++) {
					auto bits = m_ptr[m_idx];
					bits &= mask;
					for (auto idx = std::countr_zero(bits); bits; bits &= (bits - 1)) { //turns of rightmost bit (Hacker's Delight)
						idx = std::countr_zero(bits);
						m_bitIdx = idx;
						return *this;
					}
					mask = ~bitType{ 0ul };
				}

				auto bits = m_ptr[m_idx] & tailBitsMask & mask;
				for (auto idx = std::countr_zero(bits); bits; bits &= (bits - 1)) { //turns of rightmost bit (Hacker's Delight)
					idx = std::countr_zero(bits);
					m_bitIdx = idx;
					return *this;
				}

				m_idx = typeSize;
				m_bitIdx = bitsPerWord;
				return *this;
			}

			Iterator operator++(int) noexcept { Iterator tmp = *this; ++(*this); return tmp; }

			friend bool operator== (const Iterator& a, const Iterator& b) noexcept { return a.m_ptr == b.m_ptr && a.m_bitIdx == b.m_bitIdx && a.m_idx == b.m_idx; };
			friend bool operator!= (const Iterator& a, const Iterator& b) noexcept { return a.m_ptr != b.m_ptr || a.m_bitIdx != b.m_bitIdx || a.m_idx != b.m_idx; };

			Iterator() noexcept = default;
			Iterator(const bitType* p, std::uint32_t idx, std::uint32_t bitIdx) noexcept :
				m_ptr(p),
				m_idx(idx),
				m_bitIdx(bitIdx)
			{
			}
			Iterator(const Iterator& other) noexcept = default;
		private:
			const bitType* m_ptr{ nullptr };
			std::uint32_t m_idx{};
			std::uint32_t m_bitIdx{};
		};

		constexpr bitset() noexcept : m_data() 
		{
			for(auto& data : m_data)
				data = 0;
		}

		constexpr bitset(bitType t) noexcept 
		{
			m_data[0] = t;
		}

		template<typename... Tail, class = std::enable_if_t<impl::are_same<T, Tail...>::value, void>>
		constexpr bitset(Tail... args) noexcept 
		{
			*this = (*this | ... | args);
		}

		[[nodiscard]] constexpr bool test(T _idx) const noexcept 
		{
			const size_t idx = static_cast<size_t>(_idx);
			::assert(idx < bitSize);
			auto& word = m_data[idx >> bitsPerWordBitPos];
			const auto bit = 1ull << (idx & bitsMask);
			return static_cast<decltype(bit)>(word) & bit;
		}

		[[nodiscard]] constexpr bool has_single_bit() const noexcept 
		{
			return popcount() == 1ull;
		}

		[[nodiscard]] constexpr bool only(T idx) const noexcept
		{
			::assert(static_cast<size_t>(idx) < bitSize);
			return test(idx) && has_single_bit();
		}

		[[nodiscard]] constexpr Iterator end() const noexcept
		{
			static_assert(bitsPerWord <= std::numeric_limits<std::uint32_t>::max());
			static_assert(typeSize <= std::numeric_limits<std::uint32_t>::max());
			//for (std::uint32_t i = typeSize - 1; i > 0; --i) {
			//	auto bits = m_data[i];
			//	for (std::uint32_t idx = std::countl_zero(bits); bits; bits = (bits << (idx + 1)) >> (idx)) { //turns of rightmost bit (Hacker's Delight)
			//		idx = std::countl_zero(bits);
			//		return Iterator{ m_data.data(), i, static_cast<uint32_t>(bitsPerWord) - idx };
			//	}
			//}
			return Iterator{ m_data.data(), typeSize, bitsPerWord};
		}

		[[nodiscard]] constexpr Iterator begin() const noexcept
		{
			static_assert(typeSize <= std::numeric_limits<std::uint32_t>::max());
			std::uint32_t i = 0;
			for (; i < (typeSize - 1); i++) {
				auto bits = m_data[i];
				auto offset = bitsPerWord * i;
				for (std::uint32_t idx = std::countr_zero(bits); bits; bits &= (bits - 1)) { //turns of rightmost bit (Hacker's Delight)
					idx = std::countr_zero(bits);
					return Iterator{m_data.data(), i, idx};
				}
			}

			auto bits = m_data[i] & tailBitsMask;
			auto offset = bitsPerWord * i;
			for (std::uint32_t idx = std::countr_zero(bits); bits; bits &= (bits - 1)) { //turns of rightmost bit (Hacker's Delight)
				idx = std::countr_zero(bits);
				return Iterator{ m_data.data(), i, idx };
			}

			return Iterator{ m_data.data(), typeSize, bitsPerWord };
		}


		constexpr bool any_of(bitset other) const noexcept 
		{
			size_t i = 0;
			for (; i < (typeSize - 1); i++)
				if ((other.m_data[i] & m_data[i]) != bitType{ 0u })
					return true;
			
			return (other.m_data[i] & m_data[i] & tailBitsMask);
		}

		template<typename... Tail, class = std::enable_if_t<impl::are_same<T, Tail...>::value, void>>
		constexpr bool any_of(Tail... args) const noexcept 
		{
			bitset flags{ args... };
			return any_of(flags);
		}


		constexpr bool all_of(bitset other) const noexcept 
		{
			size_t i = 0;
			for (; i < (typeSize - 1); i++)
				if ((other.m_data[i] & m_data[i]) != other.m_data[i])
					return false;

			return (other.m_data[i] & m_data[i] & tailBitsMask) == (other.m_data[i] & tailBitsMask);
		}

		template<typename... Tail, class = std::enable_if_t<impl::are_same<T, Tail...>::value, void>>
		constexpr bool all_of(Tail... args) const noexcept 
		{
			bitset flags{ args... };
			return all_of(flags);
		}

		template<typename... Tail, class = std::enable_if_t<impl::are_same<T, Tail...>::value, void>>
		constexpr bitset get_and_clear(Tail... args) noexcept 
		{
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
		constexpr void for_each(Fun&& fun) const noexcept 
		{
			size_t i = 0;
			for (; i < (typeSize - 1); i++) {
				auto bits = m_data[i];
				auto offset = bitsPerWord * i;
				for (auto idx = std::countr_zero(bits); bits; bits &= (bits - 1)) { //turns of rightmost bit (Hacker's Delight)
					idx = std::countr_zero(bits);
					fun(static_cast<T>(idx + offset));
				}
			}
			auto bits = m_data[i] & tailBitsMask;
			auto offset = bitsPerWord * i;
			for (auto idx = std::countr_zero(bits); bits; bits &= (bits - 1)) { //turns of rightmost bit (Hacker's Delight)
				idx = std::countr_zero(bits);
				fun(static_cast<T>(idx + offset));
			}
		}
		constexpr void fill(std::span<T> dst) const noexcept 
		{
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
		constexpr bitset& set() noexcept 
		{
			for (size_t i = 0; i < typeSize; i++) {
				m_data[i] = static_cast<bitType>(~bitType{ 0 });
			}
			return *this;
		}
		constexpr bitset& set(T _idx) noexcept 
		{
			const size_t idx = static_cast<size_t>(_idx);
			::assert(idx < bitSize);
			auto& word = m_data[idx >> bitsPerWordBitPos];
			const auto bit = 1ull << (idx & bitsMask);
			word |= static_cast<bitType>(bit);
			return *this;
		}
		constexpr bitset& set(T _idx, bool value) noexcept 
		{
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
		constexpr bitset& reset() noexcept 
		{
			for (size_t i = 0; i < typeSize; i++) {
				m_data[i] = 0u;
			}
			return *this;
		}
		constexpr bitset& reset(T _idx) noexcept 
		{
			const size_t idx = static_cast<size_t>(_idx);
			::assert(idx < bitSize);
			auto& word = m_data[idx >> bitsPerWordBitPos];
			const auto bit = 1ull << (idx & bitsMask);
			word &= ~bit;
			return *this;
		}
		[[nodiscard]] constexpr operator bool() const noexcept 
		{
			return has_any();
		}
		[[nodiscard]] constexpr size_t popcount() const noexcept 
		{
			size_t ret = 0;
			size_t i = 0;
			for (; i < (typeSize - 1); i++)
				ret += std::popcount(m_data[i]);

			ret += std::popcount(m_data[i] & tailBitsMask);

			return ret;
		}
		[[nodiscard]] constexpr unsigned long long to_ullong() const noexcept 
		{
			if constexpr (typeSize == 0)
				return 0;
			else if constexpr (typeSize == 1)
				return m_data[0] & tailBitsMask;
			else
				return m_data[0];
			
		}
		[[nodiscard]] constexpr unsigned long to_ulong() const noexcept 
		{
			if constexpr (typeSize == 0)
				return 0;
			else if constexpr (typeSize == 1)
				return m_data[0] & tailBitsMask;
			else
				return m_data[0];

		}
		[[nodiscard]] constexpr size_t size() const noexcept 
		{
			return popcount();
		}
		[[nodiscard]] static constexpr size_t capacity() noexcept 
		{
			return bitSize;
		}
		[[nodiscard]] constexpr bool has_any() const noexcept 
		{
			size_t i = 0;
			for (; i < (typeSize - 1); i++)
				if (m_data[i] != 0)
					return true;
			
			return (m_data[i] & tailBitsMask) != bitType{ 0u };
		}
		[[nodiscard]] constexpr bool has_none() const noexcept 
		{
			size_t i = 0;
			for (; i < (typeSize - 1); i++)
				if (m_data[i] != bitType{ 0u })
					return false;
			
			return (m_data[i] & tailBitsMask) == bitType{ 0u };
		}
		[[nodiscard]] constexpr bool has_all() const noexcept 
		{
			if constexpr (bitSize == 0)
				return true;

			size_t i = 0;
			for (; i < (typeSize - 1); i++)
				if (m_data[i] != ~bitType{ 0u })
					return false;

			return (m_data[i] & tailBitsMask) == tailBitsMask;
		}

		constexpr bitset& flip() noexcept 
		{
			for (size_t i = 0; i < typeSize; i++)
				m_data[i] = ~m_data[i];
			
			return *this;
		}
		constexpr bitset& flip(T _idx) noexcept 
		{
			const size_t idx = static_cast<size_t>(_idx);
			::assert(idx < bitSize);
			auto& word = m_data[idx >> bitsPerWordBitPos];
			const auto bit = 1ull << (idx & bitsMask);
			word ^= static_cast<bitType>(bit);
			return *this;
		}
		constexpr bitset operator~() const noexcept 
		{
			return bitset(*this).flip();
		}
		constexpr bitset& operator^=(const bitset& rhs) noexcept 
		{
			for (size_t i = 0; i < typeSize; i++)
				m_data[i] ^= rhs.m_data[i];

			return *this;
		}
		constexpr bitset& operator&=(const bitset& rhs)  noexcept 
		{
			for (size_t i = 0; i < typeSize; i++)
				m_data[i] &= rhs.m_data[i];
			
			return *this;
		}
		constexpr bitset& operator|=(const bitset& rhs) noexcept 
		{
			for (size_t i = 0; i < typeSize; i++)
				m_data[i] |= rhs.m_data[i];

			return *this;
		}
		constexpr bool operator==(const bitset& rhs) const noexcept 
		{
			if constexpr (bitSize == 0)
				return true;

			bool result = true;
			size_t i = 0;
			for (; i < (typeSize - 1); i++)
				result &= m_data[i] == rhs.m_data[i];

			result &= (m_data[i] & tailBitsMask) == (rhs.m_data[i] & tailBitsMask);
			return result;
		}
		constexpr bitset<bitSize, T> operator!=(const bitset<bitSize, T>& rhs) const noexcept 
		{
			return !(*this == rhs);
		}

		constexpr bitset<bitSize, T> operator^(const bitset<bitSize, T>& rhs) const noexcept 
		{
			bitset<bitSize, T> ret = *this;
			return ret ^= rhs;
		}

		constexpr bitset<bitSize, T> operator&(const bitset<bitSize, T>& rhs) const noexcept 
		{
			bitset<bitSize, T> ret = *this;
			return ret &= rhs;
		}

		constexpr bitset<bitSize, T> operator|(const bitset<bitSize, T>& rhs) const noexcept 
		{
			bitset<bitSize, T> ret = *this;
			return ret |= rhs;
		}

		constexpr bitset<bitSize, T> operator|(const T& rhs) const noexcept 
		{
			bitset<bitSize, T> ret;
			ret.set(rhs);
			return ret |= *this;
		}
	private:
		std::array<bitType, typeSize> m_data{};
	};
}