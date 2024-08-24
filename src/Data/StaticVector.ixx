module;

#include <array>
#include <expected>
#include <bit>
#include <cassert>
#include <optional>
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

		struct Iterator
		{
			using iterator_category = std::contiguous_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = value_type;
			using pointer = pointer;  // or also value_type*
			using reference = reference;  // or also value_type&

			reference operator*() const noexcept { return *m_ptr; }
			pointer operator->() const noexcept { return m_ptr; }

			Iterator& operator++() noexcept { ++m_ptr; return *this; }

			Iterator operator++(int) noexcept { Iterator tmp = *this; ++(*this); return tmp; }

			Iterator& operator--() noexcept { --m_ptr; return *this; }

			Iterator operator--(int) noexcept { Iterator tmp = *this; --(*this); return tmp; }

			Iterator& operator-=(size_type b) noexcept
			{
				*this = *this - b;
				return *this;
			}

			Iterator& operator+=(size_type b) noexcept
			{
				*this = *this + b;
				return *this;
			}

			reference& operator[](size_type b) noexcept
			{
				return *(*this + b);
			}

			friend bool operator== (const Iterator& a, const Iterator& b) noexcept { return a.m_ptr == b.m_ptr; };
			friend bool operator!= (const Iterator& a, const Iterator& b) noexcept { return a.m_ptr != b.m_ptr; };
			friend bool operator<(const Iterator& a, const Iterator& b) noexcept { return a.m_ptr < b.m_ptr; };
			friend bool operator>(const Iterator& a, const Iterator& b) noexcept { return a.m_ptr > b.m_ptr; };
			friend bool operator<=(const Iterator& a, const Iterator& b) noexcept { return a.m_ptr <= b.m_ptr; };
			friend bool operator>=(const Iterator& a, const Iterator& b) noexcept { return a.m_ptr >= b.m_ptr; };
			friend difference_type operator-(const Iterator& a, const Iterator& b) noexcept { return a.m_ptr - b.m_ptr; };
			friend Iterator operator-(const Iterator& a, size_type b) noexcept { return a.m_ptr - b; };
			friend Iterator operator+(const Iterator& a, size_type b) noexcept { return a.m_ptr + m_ptr; };
			friend Iterator operator+(size_type b, const Iterator& a) noexcept { return a.m_ptr + m_ptr; };

			Iterator() noexcept = default;
			Iterator(const pointer p) noexcept :
				m_ptr(p)
			{

			}
			Iterator(const Iterator& other) noexcept = default;
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

			const_reference operator*() const noexcept { return *m_ptr; }
			const_pointer operator->() const noexcept { return m_ptr; }

			Const_Iterator& operator++() noexcept { ++m_ptr; return *this; }

			Const_Iterator operator++(int)noexcept { Const_Iterator tmp = *this; ++(*this); return tmp; }

			Const_Iterator& operator--() noexcept { --m_ptr; return *this; }

			Const_Iterator operator--(int) noexcept { Const_Iterator tmp = *this; --(*this); return tmp; }

			Const_Iterator& operator-=(size_type b) noexcept
			{
				*this = *this - b;
				return *this;
			}
			Const_Iterator& operator+=(size_type b) noexcept
			{
				*this = *this + b;
				return *this;
			}

			const_reference& operator[](size_type b) const noexcept
			{
				return *(*this + b);
			}


			friend bool operator== (const Const_Iterator& a, const Const_Iterator& b) noexcept { return a.m_ptr == b.m_ptr; };
			friend bool operator!= (const Const_Iterator& a, const Const_Iterator& b) noexcept { return a.m_ptr != b.m_ptr; };
			friend bool operator<(const Const_Iterator& a, const Const_Iterator& b) noexcept { return a.m_ptr < b.m_ptr; };
			friend bool operator>(const Const_Iterator& a, const Const_Iterator& b) noexcept { return a.m_ptr > b.m_ptr; };
			friend bool operator<=(const Const_Iterator& a, const Const_Iterator& b) noexcept { return a.m_ptr <= b.m_ptr; };
			friend bool operator>=(const Const_Iterator& a, const Const_Iterator& b) noexcept { return a.m_ptr >= b.m_ptr; };
			friend difference_type operator-(const Const_Iterator& a, const Const_Iterator& b) noexcept { return a.m_ptr - b.m_ptr; };
			friend Const_Iterator operator-(const Const_Iterator& a, size_type b) noexcept { return a.m_ptr - b; };
			friend Const_Iterator operator+(const Const_Iterator& a, size_type b) noexcept { return a.m_ptr + b; };
			friend Const_Iterator operator+(size_type b, const Const_Iterator& a) noexcept { return a.m_ptr + b; };

			Const_Iterator() noexcept = default;
			Const_Iterator(const const_pointer p) noexcept :
				m_ptr(p)
			{

			}
			Const_Iterator(const Const_Iterator& other) noexcept = default;
		private:

			const_pointer m_ptr{ nullptr };
		};
	public:
		constexpr StaticVector() noexcept = default;
		constexpr ~StaticVector() noexcept
		{
			clear();
		}

		void pop_back() noexcept
		{
			if (!m_size)
				return;
			--m_size;

			if constexpr (!std::is_trivially_destructible_v<value_type>)
				operator[](m_size).~value_type();

		}

		template<typename = std::enable_if_t<std::is_move_constructible_v<value_type> || (std::is_move_assignable_v<value_type> && std::is_default_constructible_v<value_type>)>>
		[[nodiscard]] bool push_back(value_type&& value) noexcept
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
		[[nodiscard]] bool push_back(const value_type& value) noexcept
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
		[[nodiscard]] bool emplace_back(Args&&... args) noexcept
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

		[[nodiscard]] size_type size() const noexcept
		{
			return m_size;
		}

		[[nodiscard]] bool empty() const noexcept
		{
			return !m_size;
		}


		[[nodiscard]] reference operator[](size_type idx) noexcept
		{
			assert(m_size > idx);
			return data()[idx];
		}

		[[nodiscard]] const_reference operator[](size_type idx) const noexcept
		{
			assert(m_size > idx);
			return data()[idx];
		}

		[[nodiscard]] reference front() noexcept
		{
			return operator[](0);
		}

		[[nodiscard]] const_reference front() const noexcept
		{
			return operator[](0);
		}

		[[nodiscard]] reference back() noexcept
		{
			return operator[](m_size - 1);
		}

		[[nodiscard]] const_reference back() const noexcept
		{
			return operator[](m_size - 1);
		}

		[[nodiscard]] pointer data() noexcept
		{
			return reinterpret_cast<pointer>(m_data.data());
		}

		[[nodiscard]] const_pointer data() const noexcept
		{
			return reinterpret_cast<const_pointer>(m_data.data());
		}

		[[nodiscard]] Iterator begin() noexcept
		{
			return Iterator(data());
		}
		[[nodiscard]] Iterator end() noexcept
		{
			return Iterator(data() + m_size);
		}

		[[nodiscard]] Const_Iterator cbegin() const noexcept
		{
			return Const_Iterator(data());
		}
		[[nodiscard]] Const_Iterator cend() const noexcept
		{
			return Const_Iterator(data() + m_size);
		}

		void clear() noexcept
		{
			static_assert(std::is_nothrow_destructible_v<value_type>);
			if constexpr (!std::is_trivially_destructible<T>())
				for (size_type i = 0; i < m_size; ++i)
					operator[](i).~value_type();
			m_size = 0;
		}
	private:

		alignas(alignof(T)) std::array<std::byte, sizeof(T)* Capacity> m_data;
		size_type m_size{ 0 };
	};
}