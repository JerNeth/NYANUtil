import NYANData;

#include <gtest/gtest.h>

namespace nyan
{
    TEST(DynamicBitsetTests, reserve) {
        DynamicBitset dynBitset;
        EXPECT_TRUE(dynBitset.reserve(64));
        EXPECT_EQ(dynBitset.capacity(), 64);

    }
    TEST(DynamicBitsetTests, test) {
        {
            DynamicBitset dynBitset;
            constexpr auto testVal = 699;
            constexpr auto testSize = 1 << 10;
            ASSERT_TRUE(dynBitset.reserve(testSize));
            dynBitset.set(testVal);
            for (auto i = 0; i < testSize; ++i) {
                if (i != testVal)
                    EXPECT_FALSE(dynBitset.test(i));
                else
                    EXPECT_TRUE(dynBitset.test(i));
            }
        }
        {

            DynamicBitset dynBitset;
            constexpr auto testVal = 62;
            constexpr auto testSize = 1 << 10;
            ASSERT_TRUE(dynBitset.reserve(testSize));
            for (auto i = 0; i < testSize; ++i) {
                dynBitset.set(i);
            }
            dynBitset.reset(testVal);
            for (auto i = 0; i < testSize; ++i) {
                if (i != testVal)
                    EXPECT_TRUE(dynBitset.test(i));
                else
                    EXPECT_FALSE(dynBitset.test(i));
            }
        }
    }

    TEST(DynamicBitsetTests, complexTest) {
        std::vector<uint64_t> testVals{ 5, 9, 19, 63, 120, 180, 251, 290, 399};
        DynamicBitset dynBitset;
        constexpr auto testSize = 1 << 10;
        ASSERT_TRUE(dynBitset.reserve(testSize));
        for (auto testVal : testVals) {
            dynBitset.set(testVal);
        }
        for (auto i = 0; i < testSize; ++i) {
            if (std::find(testVals.begin(), testVals.end(), i) == testVals.end())
                EXPECT_FALSE(dynBitset.test(i));
            else
                EXPECT_TRUE(dynBitset.test(i));
        }
        EXPECT_EQ(dynBitset.popcount(), testVals.size());
    }
    TEST(DynamicBitsetTests, fill) {
        DynamicBitset dynBitset;

        constexpr auto testSize = 1 << 10;
		ASSERT_TRUE(dynBitset.reserve(testSize));
        for (auto i = 0; i < testSize; ++i) {
            auto idx = dynBitset.find_empty();
            ASSERT_TRUE(idx) << i;
            if (idx)
                dynBitset.set(*idx);
        }
        EXPECT_GE(dynBitset.capacity(), testSize);
        EXPECT_LE(dynBitset.capacity(), testSize + 64);
        for (auto i = 0; i < testSize; ++i) {
            EXPECT_TRUE(dynBitset.test(i));
        }
        for (auto i = 0; i < testSize; ++i) {
            dynBitset.reset(i);
        }
        for (auto i = 0; i < testSize; ++i) {
            EXPECT_FALSE(dynBitset.test(i));
        }
    }
    TEST(DynamicBitsetTests, copy) {
        DynamicBitset dynBitset;

        constexpr auto testSize = 1 << 10;
        ASSERT_TRUE(dynBitset.reserve(testSize));
        for (auto i = 0; i < testSize; ++i) {
            auto idx = dynBitset.find_empty();
            ASSERT_TRUE(idx) << i;
            if (idx)
                dynBitset.set(*idx);
        }
        EXPECT_GE(dynBitset.capacity(), testSize);
        EXPECT_LE(dynBitset.capacity(), testSize + 64);
        for (auto i = 0; i < testSize; ++i) {
            EXPECT_TRUE(dynBitset.test(i));
        }
        DynamicBitset bitsetCopy{ dynBitset };
        EXPECT_GE(bitsetCopy.capacity(), testSize);
        EXPECT_LE(bitsetCopy.capacity(), testSize + 64);
        for (auto i = 0; i < testSize; ++i) {
            EXPECT_TRUE(bitsetCopy.test(i));
        }

        DynamicBitset bitsetAssignCopy = bitsetCopy;

        EXPECT_GE(bitsetAssignCopy.capacity(), testSize);
        EXPECT_LE(bitsetAssignCopy.capacity(), testSize + 64);
        for (auto i = 0; i < testSize; ++i) {
            EXPECT_TRUE(bitsetAssignCopy.test(i));
        }
    }
    TEST(DynamicBitsetTests, move) {
        DynamicBitset dynBitset;

        constexpr auto testSize = 1 << 10;
        ASSERT_TRUE(dynBitset.reserve(testSize));
        for (auto i = 0; i < testSize; ++i) {
            auto idx = dynBitset.find_empty();
            ASSERT_TRUE(idx) << i;
            if (idx)
                dynBitset.set(*idx);
        }
        EXPECT_GE(dynBitset.capacity(), testSize);
        EXPECT_LE(dynBitset.capacity(), testSize + 64);
        for (auto i = 0; i < testSize; ++i) {
            EXPECT_TRUE(dynBitset.test(i));
        }
        DynamicBitset bitsetCopy{ std::move(dynBitset) };
        EXPECT_GE(bitsetCopy.capacity(), testSize);
        EXPECT_LE(bitsetCopy.capacity(), testSize + 64);
        for (auto i = 0; i < testSize; ++i) {
            EXPECT_TRUE(bitsetCopy.test(i));
        }

        DynamicBitset bitsetAssignCopy = std::move(bitsetCopy);

        EXPECT_GE(bitsetAssignCopy.capacity(), testSize);
        EXPECT_LE(bitsetAssignCopy.capacity(), testSize + 64);
        for (auto i = 0; i < testSize; ++i) {
            EXPECT_TRUE(bitsetAssignCopy.test(i));
        }
    }
}