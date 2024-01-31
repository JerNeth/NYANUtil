import NYANData;

#include <gtest/gtest.h>

namespace nyan::util::data
{
    TEST(DynamicBitsetTests, reserve) {
        DynamicBitset dynBitset;
        EXPECT_TRUE(dynBitset.reserve(64));
        EXPECT_EQ(dynBitset.capacity(), 64);

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