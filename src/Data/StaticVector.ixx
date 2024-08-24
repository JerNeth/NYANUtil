module;

#include <array>
#include <bit>
#include <cassert>
#include <cstdlib>
#include <vector>

export module NYANData:StaticVector;
import :CommonConcepts;

export namespace nyan::util::data
{
	template<NoexceptionType T, size_t Capacity>
	class StaticVector
	{
	public:
		using size_type = std::conditional_t< Capacity <= std::numeric_limits<uint8_t>::max(), uint8_t, std::conditional_t < Capacity <= std::numeric_limits<uint16_t>::max(), uint16_t, std::conditional_t < Capacity <= std::numeric_limits<uint32_t>::max(), uint32_t, uint64_t>>>;
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using difference_type = ptrdiff_t;
	private:
		using array_type = std::array<std::byte, sizeof(value_type)* Capacity>;
	public:

		struct Iterator
		{
			using iterator_category = std::contiguous_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = value_type;
			using pointer = pointer;  // or also value_type*
			using reference = reference;  // or also value_type&

			constexpr reference operator*() const noexcept { return *m_ptr; }
			constexpr pointer operator->() const noexcept { return m_ptr; }

			constexpr Iterator& operator++() noexcept { ++m_ptr; return *this; }

			constexpr Iterator operator++(int) noexcept { Iterator tmp = *this; ++(*this); return tmp; }

			constexpr Iterator& operator--() noexcept { --m_ptr; return *this; }

			constexpr Iterator operator--(int) noexcept { Iterator tmp = *this; --(*this); return tmp; }

			constexpr Iterator& operator-=(size_type b) noexcept
			{
				*this = *this - b;
				return *this;
			}

			constexpr Iterator& operator+=(size_type b) noexcept
			{
				*this = *this + b;
				return *this;
			}

			constexpr reference& operator[](size_type b) noexcept
			{
				return *(*this + b);
			}

			static constexpr friend bool operator== (const Iterator& a, const Iterator& b) noexcept { return a.m_ptr == b.m_ptr; };
			static constexpr friend bool operator!= (const Iterator& a, const Iterator& b) noexcept { return a.m_ptr != b.m_ptr; };
			static constexpr friend bool operator<(const Iterator& a, const Iterator& b) noexcept { return a.m_ptr < b.m_ptr; };
			static constexpr friend bool operator>(const Iterator& a, const Iterator& b) noexcept { return a.m_ptr > b.m_ptr; };
			static constexpr friend bool operator<=(const Iterator& a, const Iterator& b) noexcept { return a.m_ptr <= b.m_ptr; };
			static constexpr friend bool operator>=(const Iterator& a, const Iterator& b) noexcept { return a.m_ptr >= b.m_ptr; };
			static constexpr friend difference_type operator-(const Iterator& a, const Iterator& b) noexcept { return a.m_ptr - b.m_ptr; };
			static constexpr friend Iterator operator-(const Iterator& a, size_type b) noexcept { return a.m_ptr - b; };
			static constexpr friend Iterator operator+(const Iterator& a, size_type b) noexcept { return a.m_ptr + m_ptr; };
			static constexpr friend Iterator operator+(size_type b, const Iterator& a) noexcept { return a.m_ptr + m_ptr; };

			constexpr Iterator() noexcept = default;
			constexpr Iterator(const pointer p) noexcept :
				m_ptr(p)
			{

			}
			constexpr Iterator(const Iterator& other) noexcept = default;
		private:

			pointer m_ptr{ nullptr };
		};
		struct Const_Iterator
		{
			using iterator_category = std::contiguous_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = value_type;
			using const_pointer = const_pointer;  // or also value_type*
			using const_reference = const_reference;  // or also value_type&

			constexpr const_reference operator*() const noexcept { return *m_ptr; }
			constexpr const_pointer operator->() const noexcept { return m_ptr; }

			constexpr Const_Iterator& operator++() noexcept { ++m_ptr; return *this; }

			constexpr Const_Iterator operator++(int)noexcept { Const_Iterator tmp = *this; ++(*this); return tmp; }

			constexpr Const_Iterator& operator--() noexcept { --m_ptr; return *this; }

			constexpr Const_Iterator operator--(int) noexcept { Const_Iterator tmp = *this; --(*this); return tmp; }

			constexpr Const_Iterator& operator-=(size_type b) noexcept
			{
				*this = *this - b;
				return *this;
			}
			constexpr Const_Iterator& operator+=(size_type b) noexcept
			{
				*this = *this + b;
				return *this;
			}

			constexpr const_reference& operator[](size_type b) const noexcept
			{
				return *(*this + b);
			}


			static constexpr friend bool operator== (const Const_Iterator& a, const Const_Iterator& b) noexcept { return a.m_ptr == b.m_ptr; };
			static constexpr friend bool operator!= (const Const_Iterator& a, const Const_Iterator& b) noexcept { return a.m_ptr != b.m_ptr; };
			static constexpr friend bool operator<(const Const_Iterator& a, const Const_Iterator& b) noexcept { return a.m_ptr < b.m_ptr; };
			static constexpr friend bool operator>(const Const_Iterator& a, const Const_Iterator& b) noexcept { return a.m_ptr > b.m_ptr; };
			static constexpr friend bool operator<=(const Const_Iterator& a, const Const_Iterator& b) noexcept { return a.m_ptr <= b.m_ptr; };
			static constexpr friend bool operator>=(const Const_Iterator& a, const Const_Iterator& b) noexcept { return a.m_ptr >= b.m_ptr; };
			static constexpr friend difference_type operator-(const Const_Iterator& a, const Const_Iterator& b) noexcept { return a.m_ptr - b.m_ptr; };
			static constexpr friend Const_Iterator operator-(const Const_Iterator& a, size_type b) noexcept { return a.m_ptr - b; };
			static constexpr friend Const_Iterator operator+(const Const_Iterator& a, size_type b) noexcept { return a.m_ptr + b; };
			static constexpr friend Const_Iterator operator+(size_type b, const Const_Iterator& a) noexcept { return a.m_ptr + b; };

			constexpr Const_Iterator() noexcept = default;
			constexpr Const_Iterator(const const_pointer p) noexcept :
				m_ptr(p)
			{

			}
			constexpr Const_Iterator(const Const_Iterator& other) noexcept = default;
		private:

			const_pointer m_ptr{ nullptr };
		};
	public:
		constexpr StaticVector() noexcept = default;
		constexpr ~StaticVector() noexcept
		{
			clear();
		}

		template<typename = 
				std::enable_if_t<
					std::is_nothrow_copy_constructible_v<value_type> || (std::is_nothrow_copy_assignable_v<value_type> && std::is_nothrow_default_constructible_v<value_type>)
				>
			>
		[[nodiscard]] constexpr StaticVector copy() noexcept
		{
			if (!m_size)
				return StaticVector{};


			const size_type dataSize = m_size * sizeof(value_type);

			alignas(alignof(T)) array_type data;

			if constexpr (std::is_trivially_copyable_v<value_type>)
				std::memcpy(data.data(), m_data.data(), dataSize);
			else if constexpr (std::is_nothrow_copy_constructible_v<value_type>)
				for (size_type i = 0; i < m_size; ++i)
					std::construct_at(reinterpret_cast<value_type*>(data.data()) + i, operator[](i));
			else
				for (size_type i = 0; i < m_size; ++i)
					*std::construct_at(reinterpret_cast<value_type*>(data.data()) + i) = operator[](i);

			return StaticVector{ data, m_size};
		}

		template<typename = std::enable_if_t<std::is_nothrow_move_constructible_v<value_type>> >
		constexpr StaticVector(StaticVector&& other) noexcept
		{
			if constexpr (!std::is_trivially_move_constructible_v<value_type>) {
				for (size_t i = 0; i < other.m_size; ++i) {
					auto res = emplace_back(std::move(other[i]));
					assert(res);
				}
			}
			else {
				m_data = other.m_data;
			}
			m_size = std::exchange(other.m_size, 0);

		}

		template<typename = std::enable_if_t<std::is_nothrow_copy_constructible_v<value_type> || (std::is_nothrow_copy_assignable_v<value_type> && std::is_nothrow_default_constructible_v<value_type>)>>
		StaticVector& operator=(const StaticVector& other) noexcept 
		{
			if (this != std::addressof(other)) 
			{
				const size_type dataSize = other.m_size * sizeof(value_type);

				if constexpr (!std::is_trivially_destructible_v<value_type>)
					clear();

				if constexpr (std::is_trivially_copyable_v<value_type>)
					std::memcpy(m_data.data(), other.m_data.data(), dataSize);
				else if constexpr (std::is_nothrow_copy_constructible_v<value_type>)
					for (size_type i = 0; i < other.m_size; ++i)
						std::construct_at(reinterpret_cast<value_type*>(m_data.data()) + i, other[i]);
				else
					for (size_type i = 0; i < other.m_size; ++i)
						*std::construct_at(reinterpret_cast<value_type*>(m_data.data()) + i) = other[i];
				
			}
			return *this;
		}

		template<typename = std::enable_if_t<std::is_nothrow_move_constructible_v<value_type>> >
		constexpr StaticVector& operator=(StaticVector&& other) noexcept
		{
			if (this != std::addressof(other))
			{

				if constexpr (std::is_trivially_move_constructible_v<value_type>) {
					std::swap(m_data, other.m_data);
					std::swap(m_size, other.m_size);
				}
				else {
					clear();
					for (size_type i = 0; i < other.m_size; ++i)
						*std::construct_at(&operator[](m_size++), std::move(other[i]));
					other.m_size = 0;
				}
			}
			return *this;
		}

		constexpr void pop_back() noexcept
		{
			if (!m_size)
				return;
			--m_size;

			if constexpr (!std::is_trivially_destructible_v<value_type>)
				operator[](m_size).~value_type();

		}

		template<typename = std::enable_if_t<std::is_move_constructible_v<value_type> || (std::is_move_assignable_v<value_type> && std::is_default_constructible_v<value_type>)>>
		[[nodiscard]] constexpr bool push_back(value_type&& value) noexcept
		{
			if (m_size >= Capacity)
				return false;

			if constexpr (std::is_trivially_move_constructible_v<value_type>)
				std::memcpy(&operator[](m_size++), &value, sizeof(value_type));
			else if constexpr (std::is_move_constructible_v<value_type>)
				std::construct_at(&operator[](m_size++), std::move(value));
			else
				*std::construct_at(&operator[](m_size++)) = std::move(value);

			return true;
		}

		template<typename = std::enable_if_t<std::is_copy_constructible_v<value_type> || (std::is_copy_assignable_v<value_type> && std::is_default_constructible_v<value_type>)>>
		[[nodiscard]] constexpr bool push_back(const value_type& value) noexcept
		{
			if (m_size >= Capacity)
				return false;


			if constexpr (std::is_trivially_copy_constructible_v<value_type>)
				std::memcpy(&m_data[m_size++], &value, sizeof(value_type));
			else if constexpr (std::is_copy_constructible_v<value_type>)
				std::construct_at(&operator[](m_size++), value);
			else
				*std::construct_at(&operator[](m_size++)) = value;

			return true;
		}

		template<class... Args >
		[[nodiscard]] constexpr bool emplace_back(Args&&... args) noexcept
		{
			static_assert(std::is_nothrow_constructible_v<value_type, Args...>);
			if (m_size >= Capacity)
				return false;

			std::construct_at(&operator[](m_size++), std::forward<Args>(args)...);
			return true;
		}

		[[nodiscard]] constexpr size_type capacity() const noexcept
		{
			return Capacity;
		}

		[[nodiscard]] constexpr size_type size() const noexcept
		{
			return m_size;
		}

		[[nodiscard]] constexpr bool empty() const noexcept
		{
			return !m_size;
		}


		[[nodiscard]] constexpr reference operator[](size_type idx) noexcept
		{
			assert(m_size > idx);
			return data()[idx];
		}

		[[nodiscard]] constexpr const_reference operator[](size_type idx) const noexcept
		{
			assert(m_size > idx);
			return data()[idx];
		}

		[[nodiscard]] constexpr reference front() noexcept
		{
			return operator[](0);
		}

		[[nodiscard]] constexpr const_reference front() const noexcept
		{
			return operator[](0);
		}

		[[nodiscard]] constexpr reference back() noexcept
		{
			return operator[](m_size - 1);
		}

		[[nodiscard]] constexpr const_reference back() const noexcept
		{
			return operator[](m_size - 1);
		}

		[[nodiscard]] constexpr pointer data() noexcept
		{
			return reinterpret_cast<pointer>(m_data.data());
		}

		[[nodiscard]] constexpr const_pointer data() const noexcept
		{
			return reinterpret_cast<const_pointer>(m_data.data());
		}

		[[nodiscard]] constexpr Iterator begin() noexcept
		{
			return Iterator(data());
		}
		[[nodiscard]] constexpr Iterator end() noexcept
		{
			return Iterator(data() + m_size);
		}

		[[nodiscard]] constexpr Const_Iterator cbegin() const noexcept
		{
			return Const_Iterator(data());
		}
		[[nodiscard]] constexpr Const_Iterator cend() const noexcept
		{
			return Const_Iterator(data() + m_size);
		}

		constexpr void clear() noexcept
		{
			static_assert(std::is_nothrow_destructible_v<value_type>);
			if constexpr (!std::is_trivially_destructible<T>())
				for (size_type i = 0; i < m_size; ++i)
					operator[](i).~value_type();
			m_size = 0;
		}
	private:

		constexpr StaticVector(array_type data, size_type size) noexcept :
			m_data(std::move(data)),
			m_size(size)
		{

		}

		alignas(alignof(value_type)) array_type m_data;
		size_type m_size{ 0 };
	};
}