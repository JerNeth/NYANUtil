module;

#include <array>
#include <expected>
#include <bit>
#include <cassert>
#include <optional>

export module NYANData:DynamicBitset;

export namespace nyan::util::data
{
	class DynamicBitset {
		static constexpr size_t occupancy_size = (sizeof(size_t) * 8u);
		static constexpr size_t bitsPerWordBitPos = std::countr_zero(occupancy_size);
		static constexpr size_t full_mask = ~static_cast<size_t>(0u);
		static constexpr size_t none_mask = static_cast<size_t>(0u);
		static constexpr size_t single_mask = static_cast<size_t>(1u);
	public:
		DynamicBitset() noexcept = default;
		~DynamicBitset() noexcept {
			if (m_occupancy)
				free(m_occupancy);
		}
		DynamicBitset(const DynamicBitset& other) noexcept :
			m_size(other.m_size)
		{
			m_occupancy = static_cast<size_t*>(malloc(m_size * sizeof(size_t)));
			assert(m_occupancy);
			if (m_occupancy)
				memcpy(m_occupancy, other.m_occupancy, m_size * sizeof(size_t));
			else
				m_size = 0;
		}
		DynamicBitset(DynamicBitset&& other) noexcept :
			m_occupancy(other.m_occupancy),
			m_size(other.m_size)
		{
			other.m_occupancy = nullptr;
			other.m_size = 0;
		}
		DynamicBitset& operator=(const DynamicBitset& other) noexcept
		{
			if (this != std::addressof(other)) {
				m_size = other.m_size;
				m_occupancy = static_cast<size_t*>(realloc(m_occupancy, m_size * sizeof(size_t)));
				assert(m_occupancy);
				if (m_occupancy)
					memcpy(m_occupancy, other.m_occupancy, m_size * sizeof(size_t));
				else
					m_size = 0;
			}
			return *this;
		}
		DynamicBitset& operator=(DynamicBitset&& other) noexcept
		{
			if (this != std::addressof(other)) {
				std::swap(m_occupancy, other.m_occupancy);
				std::swap(m_size, other.m_size);
			}
			return *this;
		}
		[[nodiscard]] bool reserve(size_t new_capacity) noexcept {
			if (new_capacity > capacity()) {
				auto newSize = new_capacity / occupancy_size;
				newSize += (newSize * occupancy_size != new_capacity);
				auto data = static_cast<size_t*>(realloc(m_occupancy, newSize * sizeof(size_t)));
				if (!data)
					return false;
				m_occupancy = data;
				memset(m_occupancy + m_size, 0, (newSize - m_size) * sizeof(size_t));
				m_size = newSize;
			}
			return true;
		}
		std::optional<size_t> find_empty() noexcept
		{
			size_t bucket = 0;
			for (; !m_occupancy || m_occupancy[bucket] == full_mask; bucket++) {
				if (bucket + 1 >= m_size) {
					if (!reserve(capacity() + occupancy_size))
						return std::nullopt;
					break;
				}
			}
			return bucket * sizeof(size_t) * 8 + std::countr_one(m_occupancy[bucket]);
		}
		size_t popcount() const noexcept {
			size_t ret = 0;
			assert(m_occupancy);
			for (size_t bucket = 0; bucket < m_size; bucket++) {
				ret += std::popcount(m_occupancy[bucket]);
			}
			return ret;
		}
		constexpr void clear() noexcept {
			for (size_t bucket = 0; bucket < m_size; bucket++) {
				m_occupancy[bucket] = static_cast<size_t>(0);
			}
		}
		constexpr bool test(size_t idx) const noexcept {
			assert(m_occupancy);
			assert(idx < capacity());
			return (m_occupancy[idx >> bitsPerWordBitPos] >> (idx & full_mask)) & single_mask;
		}
		constexpr void set(size_t idx) noexcept {
			assert(m_occupancy);
			assert(idx < capacity());
			m_occupancy[idx >> bitsPerWordBitPos] |= single_mask << (idx & full_mask);
		}
		constexpr void reset(size_t idx) noexcept {
			assert(m_occupancy);
			assert(idx < capacity());
			m_occupancy[idx >> bitsPerWordBitPos] &= ~(single_mask << (idx & full_mask));
		}
		constexpr void toggle(size_t idx) noexcept {
			assert(m_occupancy);
			assert(idx < capacity());
			m_occupancy[idx >> bitsPerWordBitPos] ^= single_mask << (idx & full_mask);
		}
		constexpr size_t capacity() const noexcept {
			return m_size * sizeof(size_t) * 8;
		}
	private:

		size_t* m_occupancy = nullptr;
		size_t m_size = 0;

	};
}