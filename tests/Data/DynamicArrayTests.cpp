import NYANData;

#include <random>
#include <gtest/gtest.h>

namespace nyan
{
	struct alignas(64) test {
		int b;
		int c;
	};
	struct testB {
		int b;
		int c;
		char d;
	};
	struct testC {
		int b;
		int c;
	};
	class emplaceTestClass
	{
	public:
		explicit emplaceTestClass(uint32_t a) noexcept:
		m_a(a)
		{
			
		}
		operator uint32_t() const noexcept
		{
			return m_a;
		}
	private:
		uint32_t m_a;
	};
	TEST(DynamicArrayTests, emplaceAndClearAndSize) {

		DynamicArray<uint32_t> c;

		EXPECT_TRUE(c.emplace_back(0));
		EXPECT_EQ(c.size(), 1);

		DynamicArray<emplaceTestClass> emplaceTests;

		EXPECT_TRUE(emplaceTests.emplace_back(0));
		EXPECT_EQ(emplaceTests.size(), 1);
		EXPECT_EQ(emplaceTests[0], 0);
		EXPECT_TRUE(emplaceTests.emplace_back(1));
		EXPECT_EQ(emplaceTests.size(), 2);
		EXPECT_EQ(emplaceTests[0], 0);
		EXPECT_EQ(emplaceTests[1], 1);
		EXPECT_TRUE(emplaceTests.emplace_back(2));
		EXPECT_EQ(emplaceTests.size(), 3);
		EXPECT_EQ(emplaceTests[0], 0);
		EXPECT_EQ(emplaceTests[1], 1);
		EXPECT_EQ(emplaceTests[2], 2);

		emplaceTests.clear();
		EXPECT_EQ(emplaceTests.size(), 0);
		EXPECT_TRUE(emplaceTests.emplace_back(5));
		EXPECT_EQ(emplaceTests.size(), 1);
		EXPECT_EQ(emplaceTests[0], 5);

		emplaceTests.clear();
		EXPECT_EQ(emplaceTests.size(), 0);

		constexpr uint32_t testsize = 1 << 16;
		for(auto i = 0; i < testsize; ++i)
		{
			ASSERT_TRUE(emplaceTests.emplace_back(i));
		}
		EXPECT_EQ(emplaceTests.size(), testsize);

		for (auto i = 0; i < testsize; ++i)
		{
			EXPECT_EQ(emplaceTests[i] , i);
		}

	}
	class NonTrivialClass
	{
	public:
		explicit NonTrivialClass(uint32_t a) noexcept :
			m_a(a)
		{

		}
		NonTrivialClass(const NonTrivialClass& other) noexcept :
			m_a(other.m_a)
		{

		}
		NonTrivialClass(const NonTrivialClass&& other) noexcept :
			m_a(other.m_a)
		{

		}
		NonTrivialClass& operator=(const NonTrivialClass& other) noexcept
		{
			m_a = other.m_a;
			return *this;
		}
		NonTrivialClass& operator=(const NonTrivialClass&& other) noexcept
		{
			m_a = other.m_a;
			return *this;
		}
		~NonTrivialClass()
		{
			destructorCount++;
		}
		operator uint32_t() const noexcept
		{
			return m_a;
		}
		inline static uint32_t destructorCount = 0;
	private:
		uint32_t m_a;
	};
	class MoveOnlyClass
	{
	public:
		explicit MoveOnlyClass(uint32_t a) noexcept :
			m_a(a)
		{

		}
		MoveOnlyClass(const MoveOnlyClass& other) = delete;
		MoveOnlyClass(MoveOnlyClass && other) noexcept :
			m_a(other.m_a)
		{

		}
		MoveOnlyClass& operator=(const MoveOnlyClass& other) = delete;
		MoveOnlyClass& operator=(MoveOnlyClass&& other) noexcept
		{
			m_a = other.m_a;
			return *this;
		}
		~MoveOnlyClass() noexcept
		{
			destructorCount++;
		}
		operator uint32_t() const noexcept
		{
			return m_a;
		}
		inline static uint32_t destructorCount = 0;
	private:
		uint32_t m_a;
	};
	class MoveAssignOnlyClass
	{
	public:
		MoveAssignOnlyClass() noexcept = default;
		explicit MoveAssignOnlyClass(uint32_t a) noexcept  :
			m_a(a)
		{

		}
		MoveAssignOnlyClass(const MoveAssignOnlyClass& other) = delete;
		MoveAssignOnlyClass(MoveAssignOnlyClass&& other) = delete;
		MoveAssignOnlyClass& operator=(const MoveAssignOnlyClass& other) = delete;
		MoveAssignOnlyClass& operator=(MoveAssignOnlyClass&& other) noexcept
		{
			m_a = other.m_a;
			return *this;
		}
		~MoveAssignOnlyClass()
		{
			destructorCount++;
		}
		operator uint32_t() const noexcept
		{
			return m_a;
		}
		inline static uint32_t destructorCount = 0;
	private:
		uint32_t m_a;
	};
	TEST(DynamicArrayTests, pushBack) {

		DynamicArray<uint32_t> c;

		EXPECT_TRUE(c.push_back(0));
		EXPECT_EQ(c.size(), 1);

		DynamicArray<emplaceTestClass> pushTests;

		EXPECT_TRUE(pushTests.push_back(emplaceTestClass{ 0 }));
		EXPECT_EQ(pushTests.size(), 1);
		EXPECT_EQ(pushTests[0], 0);
		EXPECT_TRUE(pushTests.push_back(emplaceTestClass{ 1 }));
		EXPECT_EQ(pushTests.size(), 2);
		EXPECT_EQ(pushTests[0], 0);
		EXPECT_EQ(pushTests[1], 1);
		EXPECT_TRUE(pushTests.push_back(emplaceTestClass{ 2 }));
		EXPECT_EQ(pushTests.size(), 3);
		EXPECT_EQ(pushTests[0], 0);
		EXPECT_EQ(pushTests[1], 1);
		EXPECT_EQ(pushTests[2], 2);

		pushTests.clear();
		EXPECT_EQ(pushTests.size(), 0);
		EXPECT_TRUE(pushTests.push_back(emplaceTestClass{ 5 }));
		EXPECT_EQ(pushTests.size(), 1);
		EXPECT_EQ(pushTests[0], 5);

		pushTests.clear();
		EXPECT_EQ(pushTests.size(), 0);

		constexpr uint32_t testsize = 1 << 16;
		for (uint32_t i = 0; i < testsize; ++i)
		{
			ASSERT_TRUE(pushTests.push_back(emplaceTestClass{ i }));
		}
		EXPECT_EQ(pushTests.size(), testsize);

		for (auto i = 0; i < testsize; ++i)
		{
			EXPECT_EQ(pushTests[i], i);
		}

		DynamicArray<NonTrivialClass> pushNonTrivialTests;
		NonTrivialClass tmp(5);
		ASSERT_TRUE(pushNonTrivialTests.push_back(tmp));
		EXPECT_EQ(pushNonTrivialTests.size(), 1);
		EXPECT_EQ(pushNonTrivialTests[0], 5);
		ASSERT_TRUE(pushNonTrivialTests.push_back(std::move(tmp)));
		EXPECT_EQ(pushNonTrivialTests.size(), 2);
		EXPECT_EQ(pushNonTrivialTests[0], 5);
		EXPECT_EQ(pushNonTrivialTests[1], 5);
		EXPECT_EQ(NonTrivialClass::destructorCount, 0);

		DynamicArray<MoveOnlyClass> pushMoveOnlyTests;
		MoveOnlyClass tmp2(56);
		ASSERT_TRUE(pushMoveOnlyTests.push_back(std::move(tmp2)));
		EXPECT_EQ(pushMoveOnlyTests.size(), 1);
		EXPECT_EQ(pushMoveOnlyTests[0], 56);
		EXPECT_EQ(MoveOnlyClass::destructorCount, 0);

		DynamicArray<MoveOnlyClass> pushMoveOnlyTests2 = std::move(pushMoveOnlyTests);
		EXPECT_EQ(pushMoveOnlyTests2.size(), 1);
		EXPECT_EQ(pushMoveOnlyTests2[0], 56);
		EXPECT_EQ(MoveOnlyClass::destructorCount, 0);

		
		DynamicArray<MoveAssignOnlyClass> pushMoveOnlyTests3;
		MoveAssignOnlyClass tmp3(56);
		ASSERT_TRUE(pushMoveOnlyTests3.push_back(std::move(tmp3)));
		EXPECT_EQ(pushMoveOnlyTests3.size(), 1);
		EXPECT_EQ(pushMoveOnlyTests3[0], 56);
		EXPECT_EQ(MoveAssignOnlyClass::destructorCount, 0);

		pushMoveOnlyTests3.clear();
		EXPECT_EQ(MoveAssignOnlyClass::destructorCount, 1);

	}
	TEST(DynamicArrayTests, copy) {

		DynamicArray<uint32_t> c;

		EXPECT_TRUE(c.emplace_back(54444));

		auto d = c.copy();
		EXPECT_TRUE(d);
		EXPECT_EQ(c[0], (*d)[0]);
		d->clear();
		EXPECT_EQ(d->size(), 0);
		EXPECT_EQ(c.size(), 1);
		ASSERT_TRUE(d->emplace_back(599));
		EXPECT_EQ(d->size(), 1);
		EXPECT_EQ(c.size(), 1);

		EXPECT_NE(c[0], (*d)[0]);
	}
	TEST(DynamicArrayTests, constructors) {
		DynamicArray<int> a;
		DynamicArray<test> t;
		DynamicArray<testB> b;
		DynamicArray<testC> c;

		ASSERT_TRUE(a.emplace_back(99));

		DynamicArray a1{ std::move(a) };
		DynamicArray<test> l1{ std::move(t) };
		DynamicArray<testB> l2{ std::move(b) };
		DynamicArray<testC> l3{ std::move(c) };

		EXPECT_EQ(a1.size(), 1);
		EXPECT_EQ(a1[0], 99);

	}

	TEST(DynamicArrayTests, iterators) {
		DynamicArray<uint32_t> a;

		for(auto in : a)
		{
			EXPECT_FALSE(true);
		}
		uint32_t count{ 0 };
		
		constexpr uint32_t testsize = 1 << 16;
		for (auto i = 0; i < testsize; ++i)
		{
			ASSERT_TRUE(a.emplace_back(i));
		}

		for (const auto& in : a)
		{
			count++;
		}
		EXPECT_EQ(count, testsize);

		for (auto& in : a)
		{
			in+= 5;
		}
		
		for (auto i = 0; i < testsize; ++i)
		{
			EXPECT_EQ(a[i], i + 5);
		}

	}
	TEST(DynamicArrayTests, resize) {
		DynamicArray<uint32_t> a;

		ASSERT_TRUE(a.resize(6));

		EXPECT_EQ(a.size(), 6);
		for (auto b : a)
			EXPECT_EQ(b, 0);

		ASSERT_TRUE(a.resize(6, 5));

		EXPECT_EQ(a.size(), 6);
		for (auto b : a)
			EXPECT_EQ(b, 0);

		ASSERT_TRUE(a.resize(12, 5));

		EXPECT_EQ(a.size(), 12);
		for (auto b : a)
			EXPECT_TRUE(b == 0 ||b == 5);

		EXPECT_EQ(a[5], 0);
		EXPECT_EQ(a[6], 5);
		EXPECT_EQ(a[11], 5);
	}

	TEST(DynamicArrayTests, benchVsVector) {

		uint32_t iters = (1 << 10) + (std::rand()% 1024);
		

		{
			auto start = std::chrono::steady_clock::now();
			DynamicArray<NonTrivialClass> a;
			for (uint32_t i = 0; i < iters; i++) {
				if (!a.push_back(NonTrivialClass(i)))
					EXPECT_EQ(a[5], 0);
				
			}
			auto end = std::chrono::steady_clock::now();
			std::cout << "DynArray pushBack took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "microseconds\n";
		}

		{
			auto start = std::chrono::steady_clock::now();
			std::vector<NonTrivialClass> a;
			for (uint32_t i = 0; i < iters; i++) {
				a.push_back(NonTrivialClass(i));
			}
			auto end = std::chrono::steady_clock::now();
			std::cout << "Vector pushBack took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "microseconds\n";
		}
		{
			auto start = std::chrono::steady_clock::now();
			DynamicArray<NonTrivialClass> a;
			for (uint32_t i = 0; i < iters; i++) {
				if (!a.emplace_back(i))
					EXPECT_EQ(a[5], 0);
			}
			auto end = std::chrono::steady_clock::now();
			std::cout << "DynArray emplaceBack took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "microseconds\n";
		}

		{
			auto start = std::chrono::steady_clock::now();
			std::vector<NonTrivialClass> a;
			for (uint32_t i = 0; i < iters; i++) {
				a.emplace_back(i);
			}
			auto end = std::chrono::steady_clock::now();
			std::cout << "Vector emplaceBack took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "microseconds\n";
		}

		{
			DynamicArray<NonTrivialClass> a;
			for (uint32_t i = 0; i < iters; i++) {
				if (!a.emplace_back(i))
					EXPECT_EQ(a[5], 0);
			}
			NonTrivialClass::destructorCount = 0;
			auto start = std::chrono::steady_clock::now();
			a.clear();
			auto end = std::chrono::steady_clock::now();
			EXPECT_EQ(NonTrivialClass::destructorCount, iters);
			std::cout << "DynArray clear took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "microseconds\n";
		}

		{
			std::vector<NonTrivialClass> a;
			for (uint32_t i = 0; i < iters; i++) {
				a.emplace_back(i);
			}
			auto start = std::chrono::steady_clock::now();
			a.clear();
			auto end = std::chrono::steady_clock::now();
			std::cout << "Vector clear took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "microseconds\n";
		}
		{
			//DynArray<std::string> a;
			//auto start = std::chrono::steady_clock::now();
			//for (uint32_t i = 0; i < iters; i++) {
			//	if (!a.emplace_back(std::to_string(i)))
			//		EXPECT_TRUE(!a[5].compare("5"));
			//}
			//auto end = std::chrono::steady_clock::now();
			//std::cout << "DynArray clear took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "microseconds\n";
		}

		{
			std::vector<std::string> a;
			auto start = std::chrono::steady_clock::now();
			for (uint32_t i = 0; i < iters; i++) {
				a.emplace_back(std::to_string(i));
			}
			auto end = std::chrono::steady_clock::now();
			std::cout << "Vector clear took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "microseconds\n";
		}

		const uint64_t itxes = (1<<10) + (rand() & 0x7);

		{
			std::vector<uint64_t> ab;
			for (uint64_t i = 0; i < iters; i++) {
				ab.push_back(i);
			}
			uint64_t sum = 0;
			auto start = std::chrono::steady_clock::now();
			for (uint32_t itx = 0; itx < itxes; ++itx)
				for (auto as : ab)
					sum += as;
			auto end = std::chrono::steady_clock::now();
			std::cout << sum << " Vector iterate took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "microseconds\n";
		}
		{
			DynamicArray<uint64_t> ab;
			for (uint64_t i = 0; i < iters; i++) {
				if (!ab.push_back(i))
					EXPECT_EQ(1, 0);

			}
			uint64_t sum = 0;
			auto start = std::chrono::steady_clock::now();
			for (uint32_t itx = 0; itx < itxes; ++itx)
				for (auto as : ab)
					sum += as;
			auto end = std::chrono::steady_clock::now();
			std::cout << sum << " DynArray iterate took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "microseconds\n";
		}
	}
}