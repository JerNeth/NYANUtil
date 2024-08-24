import NYANData;

#include <gtest/gtest.h>

namespace nyan::util::data
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
        s.push_back(0);
        s.push_back(1);
        s.push_back(2);
        s.push_back(3);

        uint32_t sum = 0;
        for (auto a : s)
            sum += a;
        EXPECT_EQ(sum, 6);
    }
}