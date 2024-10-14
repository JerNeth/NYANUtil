module;

#include <array>
#include <concepts>
#include <expected>
#include <bit>
#include <optional>
#include <cstdlib>
#include <string_view>
#include <vector>

export module NYANData:DynamicArray;
//import std;
import NYANAssert;
import :Common;

#ifdef NDEBUG
constexpr inline auto assert = nyan::assert::Assert<nyan::assert::AssertionLevel::Disabled, nyan::assert::AssertionExitMode::Disabled, nyan::assert::AssertionLogMode::Disabled>{};
#else
constexpr inline auto assert = nyan::assert::Assert<nyan::assert::AssertionLevel::Enabled, nyan::assert::AssertionExitMode::Abort, nyan::assert::AssertionLogMode::StackTrace>{};
#endif

export namespace nyan
{
	template<NoexceptionType T>
	class DynamicArray
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using size_type = size_t;
		using difference_type = std::ptrdiff_t;

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
			friend Iterator operator+(const Iterator& a, size_type b) noexcept { return a.m_ptr + b; };
			friend Iterator operator+(size_type b, const Iterator& a) noexcept { return a.m_ptr + b; };

			Iterator() noexcept = default;
			Iterator(const pointer p) noexcept :
				m_ptr(p)
			{
				
			}
			Iterator(const Iterator& other) noexcept = default;
		private:

			pointer m_ptr {nullptr};
		};
		struct Const_Iterator
		{
			using iterator_category = std::contiguous_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = value_type;
			using const_pointer = const_pointer;  // or also value_type*
			using const_reference = const_reference;  // or also value_type&

			const_reference operator*() const noexcept{ return *m_ptr; }
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
		constexpr DynamicArray() noexcept = default;
		constexpr ~DynamicArray() noexcept
		{
			::assert(!(static_cast<bool>(m_data) ^ static_cast <bool>(m_capacity)));
			clean();
			if (m_data)
#if _MSC_VER
				_aligned_free(m_data);
#else
				free(m_data);
#endif
		}
		//Implicit copys are evil
		DynamicArray(const DynamicArray& other) = delete;

		[[nodiscard]] std::optional<DynamicArray> copy() noexcept requires std::copy_constructible< value_type> || std::is_copy_assignable_v< value_type> && std::is_default_constructible_v<value_type>
		{
			if(!m_data)
				return DynamicArray{ nullptr, 0, 0 };

			static_assert(!(sizeof(value_type) % alignof(value_type)));

			const size_type allocatedDataSize = m_capacity * sizeof(value_type);
#if _MSC_VER
			const pointer data = static_cast<pointer>(_aligned_malloc(allocatedDataSize, alignof(value_type)));
#else
			const pointer data = static_cast<pointer>(aligned_alloc(alignof(value_type), allocatedDataSize));
#endif
			if (!data)
				return std::nullopt;

			const size_type dataSize = m_size * sizeof(value_type);

			if constexpr (std::is_trivially_copyable_v<value_type>)
				std::memcpy(data, m_data, dataSize);
			else if constexpr (std::is_copy_constructible_v<value_type>)
				for (size_type i = 0; i < m_size; ++i)
					new (&m_data[i]) value_type(m_data[i]);
			else
				for (size_type i = 0; i < m_size; ++i)
					*static_cast<value_type*>(new (&m_data[i]) value_type()) = m_data[i];

			return DynamicArray{data, m_size, m_capacity};
		}

		constexpr DynamicArray(DynamicArray&& other) noexcept :
			m_data(std::exchange(other.m_data, nullptr)),
			m_size(std::exchange(other.m_size, 0)),
			m_capacity(std::exchange(other.m_capacity, 0))
		{
		}

		DynamicArray& operator=(DynamicArray& other) = delete;

		constexpr DynamicArray& operator=(DynamicArray&& other) noexcept
		{
			if(this != std::addressof(other))
			{
				std::swap(m_data, other.m_data);
				std::swap(m_size, other.m_size);
				std::swap(m_capacity, other.m_capacity);
			}
			return *this;
		}
		
		void pop_back() noexcept
		{
			if (!m_size)
				return;
			--m_size;

			if constexpr (!std::is_trivially_destructible_v<value_type>)
				m_data[m_size].~value_type();
			
		}

		[[nodiscard]] bool push_back(value_type&& value) noexcept requires std::move_constructible< value_type> || std::is_move_assignable_v< value_type> && std::is_default_constructible_v<value_type>
		{
			if (m_size >= m_capacity)
				if (!grow(calc_new_capacity()))
					return false;

			if constexpr (std::is_trivially_move_constructible_v<value_type>)
				std::memcpy(&m_data[m_size++], &value, sizeof(value_type));
			else if constexpr (std::is_move_constructible_v<value_type>)
				std::construct_at(&m_data[m_size++], std::move(value));
			else
				*std::construct_at(&m_data[m_size++]) = std::move(value);

			return true;
		}

		[[nodiscard]] bool push_back(const value_type& value) noexcept requires std::copy_constructible< value_type> || std::is_copy_assignable_v< value_type> && std::is_default_constructible_v<value_type>
		{
			if (m_size >= m_capacity)
				if (!grow(calc_new_capacity()))
					return false;
			

			if constexpr (std::is_trivially_copy_constructible_v<value_type>)
				std::memcpy(&m_data[m_size++], &value, sizeof(value_type));
			else if constexpr (std::is_copy_constructible_v<value_type>)
				std::construct_at(&m_data[m_size++], value);
			else
				*std::construct_at(&m_data[m_size++]) = value;

			return true;
		}

		template<class... Args >
		[[nodiscard]] bool emplace_back(Args&&... args) noexcept
		{
			static_assert(std::is_nothrow_constructible_v<value_type, Args...>);
			if (m_size >= m_capacity)
				if (!grow(calc_new_capacity()))
					return false;

			std::construct_at(&m_data[m_size++], std::forward<Args>(args)...);
			return true;
		}
		
		constexpr void clear() noexcept
		{
			clean();
		}


		[[nodiscard]] bool resize(const size_type count) noexcept requires std::is_default_constructible_v< value_type>
		{
			if (m_size == count)
				return true;

			if (m_size < count) {
				if (!grow(count))
					return false;

				for (; m_size < count; ++m_size)
					std::construct_at(&m_data[m_size]);
				return true;
			}

			if constexpr (!std::is_trivially_destructible_v<value_type>)
				while (m_size > count)
					m_data[--m_size].~value_type();
			else
				m_size = count;

			return true;
		}

		[[nodiscard]] bool resize(const size_type count, const value_type& value) noexcept requires std::copy_constructible< value_type>
		{
			if (m_size == count)
				return true;

			if (m_size < count) {
				if (!grow(count))
					return false;

				for (; m_size < count; ++m_size)
					std::construct_at(&m_data[m_size], value);
				return true;
			}

			if constexpr (!std::is_trivially_destructible_v<value_type>)
				while (m_size > count)
					m_data[--m_size].~value_type();
			else
				m_size = count;

			return true;
		}

		[[nodiscard]] bool reserve(size_type size) noexcept
		{
			return grow(size);
		}

		[[nodiscard]] size_type capacity() const noexcept
		{
			return m_capacity;
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
			::assert(m_data);
			::assert(m_size > idx);
			return m_data[idx];
		}

		[[nodiscard]] const_reference operator[](size_type idx) const noexcept
		{
			::assert(m_data);
			::assert(m_size > idx);
			return m_data[idx];
		}

		[[nodiscard]] reference front() noexcept
		{
			return m_data[0];
		}

		[[nodiscard]] const_reference front() const noexcept
		{
			return m_data[0];
		}

		[[nodiscard]] reference back() noexcept
		{
			return m_data[m_size - 1];
		}

		[[nodiscard]] const_reference back() const noexcept
		{
			return m_data[m_size - 1];
		}

		[[nodiscard]] pointer data() noexcept
		{
			return m_data;
		}

		[[nodiscard]] const_pointer data() const noexcept
		{
			return m_data;
		}

		[[nodiscard]] Iterator begin() noexcept
		{
			return Iterator(m_data);
		}
		[[nodiscard]] Iterator end() noexcept
		{
			return Iterator(m_data + m_size);
		}

		[[nodiscard]] Const_Iterator cbegin() const noexcept
		{
			return Const_Iterator(m_data);
		}
		[[nodiscard]] Const_Iterator cend() const noexcept
		{
			return Const_Iterator(m_data + m_size);
		}

		friend constexpr void swap(DynamicArray& lhs, DynamicArray& rhs) noexcept
		{
			if(std::addressof(lhs) != std::addressof(rhs))
			{
				std::swap(lhs.m_data, rhs.m_data);
				std::swap(lhs.m_size, rhs.m_size);
				std::swap(lhs.m_capacity, rhs.m_capacity);
			}
		}
		
	private:
		DynamicArray(const pointer data, const size_type size, const size_type capacity) noexcept :
			m_data(data),
			m_size(size),
			m_capacity(capacity)
		{
			
		}

		bool grow(const size_type newCapacity) noexcept
		{

			if (m_capacity >= newCapacity)
				return true;

			static_assert(!(sizeof(value_type) % alignof(value_type)));
			m_capacity = newCapacity;
			const size_type allocatedDataSize = m_capacity * sizeof(value_type);

#ifdef _MSC_VER
			auto* data = static_cast<pointer>(_aligned_realloc(m_data, allocatedDataSize, alignof(value_type)));
#else
			auto* data = static_cast<pointer>(aligned_alloc(alignof(value_type), allocatedDataSize));
#endif
			if (!data)
				return false;
			std::swap(data, m_data);

#ifndef _MSC_VER
			const size_type dataSize = m_size * sizeof(value_type);
			std::memcpy(m_data, data, dataSize);
			free(data);
#endif
			
			return true;
		}

		constexpr size_t calc_new_capacity() const noexcept
		{
			auto newCapacity = m_capacity;
			newCapacity += m_capacity >> 1;
			newCapacity += m_capacity & 0x1;
			return std::max(newCapacity, 1ull);
		}

		constexpr void clean() noexcept
		{
			static_assert(std::is_nothrow_destructible_v<value_type>);
			if constexpr (!std::is_trivially_destructible_v<value_type>) {
				for (size_type i = 0; i < m_size; ++i)
					m_data[i].~T();
			}
			m_size = 0;
		}

		pointer m_data {nullptr};
		size_type m_size{ 0 };
		size_type m_capacity {0};
	};
}