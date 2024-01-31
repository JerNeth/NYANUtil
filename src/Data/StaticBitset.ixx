module;

#include <array>
#include <bit>
#include <cassert>

export module NYANData:StaticBitset;

export namespace nyan::util::data
{
	template<size_t bitSize, typename T = size_t> // typename for indices e.g. enums
	class bitset {
		using bitType = std::conditional_t< bitSize <= 8, uint8_t, std::conditional_t < bitSize <= 16, uint16_t, std::conditional_t < bitSize <= 32, uint32_t, uint64_t>>>;
		static constexpr size_t bitsPerWord = (sizeof(bitType) * 8);
		static constexpr size_t bitsPerWordBitPos = std::countr_zero(bitsPerWord);
		static_assert(std::has_single_bit(bitsPerWord));
		static constexpr size_t bitsMask = bitsPerWord - 1;
		static constexpr size_t typeSize = bitSize / bitsPerWord + (bitSize % bitsPerWord != 0);
		//static_assert(std::is_convertible<T, size_t>::value);
	public:
		constexpr bitset() noexcept : m_data() {
			m_data[0] = 0;
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
			assert(idx < bitSize);
			auto& word = m_data[idx >> bitsPerWordBitPos];
			const auto bit = 1ull << (idx & bitsMask);
			return static_cast<decltype(bit)>(word) & bit;
		}
		[[nodiscard]] constexpr bool only(T _idx) const noexcept {
			const size_t idx = static_cast<size_t>(_idx);
			assert(idx < bitSize);
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
		constexpr bitset& set() noexcept {
			for (size_t i = 0; i < typeSize; i++) {
				m_data[i] = static_cast<bitType>(~bitType{ 0 });
			}
			return *this;
		}
		constexpr bitset& set(T _idx) noexcept {
			const size_t idx = static_cast<size_t>(_idx);
			assert(idx < bitSize);
			auto& word = m_data[idx >> bitsPerWordBitPos];
			const auto bit = 1ull << (idx & bitsMask);
			word |= static_cast<bitType>(bit);
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
			assert(idx < bitSize);
			auto& word = m_data[idx >> bitsPerWordBitPos];
			const auto bit = 1ull << (idx & bitsMask);
			word &= ~bit;
			return *this;
		}
		[[nodiscard]] constexpr operator bool() const noexcept {
			return any();
		}
		[[nodiscard]] constexpr size_t count() const noexcept {
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
			return memcmp(m_data.data(), rhs.m_data.data(), typeSize * sizeof(bitType)) == 0;
		}
		template<size_t bitSize, typename T>
		friend constexpr bitset<bitSize, T> operator!=(const bitset<bitSize, T>& lhs, const bitset<bitSize, T>& rhs) {
			return !(lhs == rhs);
		}
		template<size_t bitSize, typename T>
		friend constexpr bitset<bitSize, T> operator^(const bitset<bitSize, T>& lhs, const bitset<bitSize, T>& rhs) {
			bitset<bitSize, T> ret = lhs;
			return ret ^= rhs;
		}
		template<size_t bitSize, typename T>
		friend constexpr bitset<bitSize, T> operator&(const bitset<bitSize, T>& lhs, const bitset<bitSize, T>& rhs) {
			bitset<bitSize, T> ret = lhs;
			return ret &= rhs;
		}
		template<size_t bitSize, typename T>
		friend constexpr bitset<bitSize, T> operator|(bitset<bitSize, T> lhs, const bitset<bitSize, T>& rhs) {
			return lhs |= rhs;
		}
		template<size_t bitSize, typename T>
		friend constexpr bitset<bitSize, T> operator|(const bitset<bitSize, T>& lhs, const T& rhs) {
			bitset<bitSize, T> ret;
			ret.set(rhs);
			return ret |= lhs;
		}
	private:
		std::array<bitType, typeSize> m_data{};
	};
}