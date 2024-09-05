import NYANData;

#include <gtest/gtest.h>
#include <expected>

namespace nyan
{
    TEST(StaticVectorTests, ConstructDestruct) {
        {
            StaticVector<uint32_t, 4> s;
        }
        {
            StaticVector<uint32_t, 4> s;
            EXPECT_TRUE(s.push_back({}));
        }
    }
    uint32_t constructs = 0;
    uint32_t copyC = 0;
    uint32_t moveC = 0;
    uint32_t copyA = 0;
    uint32_t moveA = 0;
    uint32_t destructs = 0;
    TEST(StaticVectorTests, ConstructDestructNonTrivial) {
        struct T {
            T() noexcept {
                constructs++;
            }
            T(T& t) noexcept {
                copyC++;
            }
            T(T&& t) noexcept {
                moveC++;
            }
            ~T() noexcept {
                destructs++;
            }
            T& operator=(T& t) noexcept
            {
                copyA++;
                return *this;
            }
            T& operator=(T&& t) noexcept
            {
                moveA++;
                return *this;
            }
        };
        {
            StaticVector<T, 4> s;
        }

        EXPECT_EQ(constructs, 0);
        EXPECT_EQ(destructs, 0);
        EXPECT_EQ(copyC, 0);
        EXPECT_EQ(moveC, 0);
        EXPECT_EQ(copyA, 0);
        EXPECT_EQ(moveA, 0);

        {
            StaticVector<T, 4> s;
            EXPECT_TRUE(s.push_back({}));
        }

        EXPECT_EQ(constructs, 1);
        EXPECT_EQ(destructs, 2);
        EXPECT_EQ(copyC, 0);
        EXPECT_EQ(moveC, 1);
        EXPECT_EQ(copyA, 0);
        EXPECT_EQ(moveA, 0);

    }

    TEST(StaticVectorTests, AccessTests) {

        StaticVector<uint32_t, 4> s;
        EXPECT_TRUE(s.empty());
        EXPECT_TRUE(s.push_back(0));
        EXPECT_EQ(s.front(), 0);
        EXPECT_EQ(s.back(), 0);
        EXPECT_TRUE(s.push_back(1));
        EXPECT_EQ(s.back(), 1);
        EXPECT_TRUE(s.push_back(2));
        EXPECT_EQ(s.back(), 2);
        EXPECT_TRUE(s.push_back(3));
        EXPECT_EQ(s.back(), 3);
        EXPECT_FALSE(s.push_back(4));

        EXPECT_EQ(s[0], 0);
        EXPECT_EQ(s[1], 1);
        EXPECT_EQ(s[2], 2);
        EXPECT_EQ(s[3], 3);
        EXPECT_FALSE(s.empty());
    }

    TEST(StaticVectorTests, Iterators) {

        StaticVector<uint32_t, 4> s;
        ASSERT_TRUE(s.push_back(0));
        ASSERT_TRUE(s.push_back(1));
        ASSERT_TRUE(s.push_back(2));
        ASSERT_TRUE(s.push_back(3));

        uint32_t sum = 0;
        for (auto a : s)
            sum += a;
        EXPECT_EQ(sum, 6);
    }

    TEST(StaticVectorTests, Move) {

        StaticVector<uint32_t, 4> s;
        ASSERT_TRUE(s.push_back(0));
        ASSERT_TRUE(s.push_back(1));
        ASSERT_TRUE(s.push_back(2));
        ASSERT_TRUE(s.push_back(3));

        StaticVector<uint32_t, 4> t{ std::move(s) };
        EXPECT_EQ(t[0], 0);
        EXPECT_EQ(t[1], 1);
        EXPECT_EQ(t[2], 2);
        EXPECT_EQ(t[3], 3);

        t[3] = 6;

        s = t.copy();

        EXPECT_EQ(s[3], 6);

        auto l = std::move(s);

        EXPECT_EQ(l[3], 6);

    }
    TEST(StaticVectorTests, MoveNonTrivial) {
        struct T {
            T() noexcept {
                data = 0;
            }
            T(uint32_t d) noexcept {
                data = d;
            }
            T(const T& t) noexcept {
                data = t.data;
            }
            T(T&& t) noexcept {
                data = t.data;
                t.data = 0;
            }
            ~T() noexcept {
                data = 9999;
            }
            T& operator=(const T& t) noexcept
            {
                data = t.data;
                return *this;
            }
            T& operator=(T&& t) noexcept
            {
                data = t.data;
                t.data = 0;
                return *this;
            }
            uint32_t data;
        };
        StaticVector<T, 4> s;
        ASSERT_TRUE(s.push_back(0));
        ASSERT_TRUE(s.push_back(1));
        ASSERT_TRUE(s.push_back(2));
        ASSERT_TRUE(s.push_back(3));

        StaticVector<T, 4> t{ std::move(s) };
        EXPECT_EQ(t[0].data, 0);
        EXPECT_EQ(t[1].data, 1);
        EXPECT_EQ(t[2].data, 2);
        EXPECT_EQ(t[3].data, 3);

        t[3].data = 6;

        s = t.copy();

        EXPECT_EQ(s[3].data, 6);

        auto l = std::move(s);

        EXPECT_EQ(l[3].data, 6);

        s = l;

        EXPECT_EQ(s[3].data, 6);

    }
    TEST(StaticVectorTests, DefaultConstruct) {

        StaticVector<uint32_t, 4> s{ 4u };
        std::array<std::byte, sizeof(s)> a;


        auto* ptr = std::construct_at(reinterpret_cast<decltype(&s)>(a.data()), 3);

        ptr->back() = 5;

        auto* ptr2 = std::construct_at(reinterpret_cast<decltype(&s)>(a.data()), 3);

        EXPECT_EQ(ptr2->size(), 3);

        EXPECT_EQ(ptr2->operator[](0), 0); 
        EXPECT_EQ(ptr2->operator[](1), 0); 
        EXPECT_EQ(ptr2->operator[](2), 0); 
    }
    TEST(StaticVectorTests, TryPushBack) {

        StaticVector<uint32_t, 4> s{};

        std::expected<uint32_t, uint64_t> a{ 0ul };
        std::expected<uint32_t, uint64_t> b = std::unexpected{ 3ull };

        auto ret = s.try_push_back(std::move(a));
        EXPECT_TRUE(ret.has_value());

        auto ret2 = s.try_push_back(std::move(b));
        EXPECT_FALSE(ret2.has_value());
        EXPECT_EQ(ret2.error(), 3ull);

    }
    TEST(StaticVectorTests, InitializerList) {

        StaticVector<uint32_t, 4> s{ {3, 4 } };

        EXPECT_EQ(s[0], 3ull);
        EXPECT_EQ(s[1], 4ull);

        StaticVector<uint32_t, 2> t{ {3, 4 } };

        EXPECT_EQ(t[0], 3ull);
        EXPECT_EQ(t[1], 4ull);

    }
    TEST(StaticVectorTests, NonTrivialMove) {
        struct T {
            T() noexcept {
                data = 0;
            }
            T(uint32_t d) noexcept {
                data = d;
            }
            T(const T& t) noexcept {
                data = t.data;
            }
            T(T&& t) noexcept {
                data = t.data;
                t.data = 100923;
            }
            ~T() noexcept {
                data = 9999;
            }
            T& operator=(const T& t) noexcept
            {
                data = t.data;
                return *this;
            }
            T& operator=(T&& t) noexcept
            {
                data = t.data;
                t.data = 100924;
                return *this;
            }
            uint32_t data;
        };
        StaticVector<T, 4> s{ {3, 4 } };

        EXPECT_EQ(s[0].data, 3ull);
        EXPECT_EQ(s[1].data, 4ull);

        auto t{std::move(s)};

        EXPECT_EQ(t[0].data, 3ull);
        EXPECT_EQ(t[1].data, 4ull);

        //EXPECT_EQ(s[0].data, 100923ull);
        //EXPECT_EQ(s[1].data, 100923ull);

    }
}