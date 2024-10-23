module;

export module NYANData:Pointer;
import std;

import :ModuleSettings;

namespace impl {
	template<typename T>
	concept Pointer = std::is_pointer_v<T>;
}

export namespace nyan
{
	template<impl::Pointer T>
	class NotNull {
	public:
		NotNull() = delete; //Can't init without a value
		constexpr NotNull(T&& ptr) noexcept : m_ptr(std::forward<T>(ptr)) { ::assert(m_ptr != nullptr); }

		template <typename = std::enable_if_t<!std::is_same_v<T, std::nullptr_t>>>
		constexpr NotNull(T ptr) noexcept : m_ptr(ptr) { ::assert(m_ptr != nullptr); }
		constexpr NotNull(const NotNull& other) noexcept : m_ptr(other.m_ptr) { ::assert(m_ptr != nullptr); }

		constexpr operator T() const noexcept { return m_ptr; };

		constexpr NotNull& operator=(const NotNull& other) noexcept {
			m_ptr = other.m_ptr;
			return *this;
		}

		constexpr decltype(auto) operator*() const noexcept { return *m_ptr; };
		constexpr decltype(auto) operator->() const noexcept { return m_ptr; };

		NotNull(std::nullptr_t) = delete;
		NotNull& operator=(std::nullptr_t) = delete;
	private:
		T m_ptr;
	};
}
