module;
//
//#include <array>
//#include <expected>
//#include <bit>
//#include <optional>
//#include <string_view>

export module NYANData:DynamicBitset;
import std;
import NYANAssert;

#ifdef NDEBUG
constexpr inline auto assert = nyan::assert::Assert<nyan::assert::AssertionLevel::Disabled, nyan::assert::AssertionExitMode::Disabled, nyan::assert::AssertionLogMode::Disabled>{};
#else
constexpr inline auto assert = nyan::assert::Assert<nyan::assert::AssertionLevel::Enabled, nyan::assert::AssertionExitMode::Abort, nyan::assert::AssertionLogMode::StackTrace>{};
#endif

export namespace nyan
{
	class DynamicBitset {
		using StorageType = unsigned long long;
		static constexpr StorageType occupancySize = (sizeof(StorageType) * 8u);
		static constexpr StorageType  fullMask = ~static_cast<StorageType>(0u);
		static constexpr StorageType  noneMask = static_cast<StorageType>(0u);
		static constexpr StorageType  singleMask = static_cast<StorageType>(1u);
		static constexpr StorageType  bitsPerWordBitPos = std::countr_zero(occupancySize);
		static constexpr StorageType  bitsDivMask = (singleMask << bitsPerWordBitPos) - 1;
	public:
		DynamicBitset() noexcept = default;
		~DynamicBitset() noexcept {
			if (m_occupancy)
				std::free(m_occupancy);
		}
		DynamicBitset(const DynamicBitset& other) noexcept :
			m_bufferSize(other.m_bufferSize)
		{
			m_occupancy = static_cast<StorageType*>(std::malloc(m_bufferSize * sizeof(StorageType)));
			::assert(m_occupancy);
			if (m_occupancy)
				std::memcpy(m_occupancy, other.m_occupancy, m_bufferSize * sizeof(StorageType));
			else
				m_bufferSize = 0;
		}
		DynamicBitset(DynamicBitset&& other) noexcept :
			m_occupancy(std::exchange(other.m_occupancy, nullptr)),
			m_bufferSize(std::exchange(other.m_bufferSize, 0))
		{
		}
		DynamicBitset& operator=(const DynamicBitset& other) noexcept
		{
			if (this != std::addressof(other)) {
				m_bufferSize = other.m_bufferSize;
				m_occupancy = static_cast<StorageType*>(std::malloc(m_bufferSize * sizeof(StorageType)));
				::assert(m_occupancy);
				if (m_occupancy)
					std::memcpy(m_occupancy, other.m_occupancy, m_bufferSize * sizeof(StorageType));
				else
					m_bufferSize = 0;
			}
			return *this;
		}
		DynamicBitset& operator=(DynamicBitset&& other) noexcept
		{
			if (this != std::addressof(other)) {
				std::swap(m_occupancy, other.m_occupancy);
				std::swap(m_bufferSize, other.m_bufferSize);
			}
			return *this;
		}
		[[nodiscard]] bool reserve(size_t new_capacity) noexcept {
			if (new_capacity > capacity()) {
				size_t newSize{ 0 };
				if constexpr(std::has_single_bit(occupancySize))
					newSize = new_capacity >> std::countr_zero(occupancySize);
				else
					newSize = new_capacity / occupancySize;

				newSize += (newSize * occupancySize != new_capacity);
				auto data = static_cast<StorageType*>(std::realloc(m_occupancy, newSize * sizeof(StorageType)));
				if (!data)
					return false;
				m_occupancy = data;
				std::memset(m_occupancy + m_bufferSize, 0, (newSize - m_bufferSize) * sizeof(StorageType));
				m_bufferSize = newSize;
			}
			return true;
		}
		std::optional<size_t> find_empty() const noexcept
		{
			size_t bucket = 0;
			for (; !m_occupancy || m_occupancy[bucket] == fullMask; bucket++) {
				
				if (bucket + 1 >= m_bufferSize) {
					return std::nullopt;
				//	if (!reserve(capacity() + occupancySize))
				//		return std::nullopt;
				//	break;
				}
			}
			::assert(m_bufferSize);
			return bucket * occupancySize + std::countr_one(m_occupancy[bucket]);
		}
		size_t popcount() const noexcept {
			size_t ret = 0;
			::assert(m_occupancy);
			for (size_t bucket = 0; bucket < m_bufferSize; bucket++) {
				ret += std::popcount(m_occupancy[bucket]);
			}
			return ret;
		}
		constexpr void clear() noexcept {
			for (size_t bucket = 0; bucket < m_bufferSize; bucket++) {
				m_occupancy[bucket] = static_cast<size_t>(0);
			}
		}
		constexpr bool test(size_t idx) const noexcept {
			::assert(m_occupancy);
			::assert(idx < capacity());
			return (m_occupancy[idx >> bitsPerWordBitPos] >> (idx & bitsDivMask)) & singleMask;
		}
		constexpr void set(size_t idx) noexcept {
			::assert(m_occupancy);
			::assert(idx < capacity());
			m_occupancy[idx >> bitsPerWordBitPos] |= singleMask << (idx & bitsDivMask);
		}
		constexpr void reset(size_t idx) noexcept {
			::assert(m_occupancy);
			::assert(idx < capacity());
			m_occupancy[idx >> bitsPerWordBitPos] &= ~(singleMask << (idx & bitsDivMask));
		}
		constexpr void toggle(size_t idx) noexcept {
			::assert(m_occupancy);
			::assert(idx < capacity());
			m_occupancy[idx >> bitsPerWordBitPos] ^= singleMask << (idx & bitsDivMask);
		}
		constexpr size_t capacity() const noexcept {
			return m_bufferSize * occupancySize;
		}
	private:

		StorageType* m_occupancy = nullptr;
		size_t m_bufferSize = 0;

	};
}