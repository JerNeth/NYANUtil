import NYANData;

#include <gtest/gtest.h>
#include <array>
#include <span>
#include <random>

namespace nyan
{
    TEST(Hashtests, ConstexprArbitraryHash) {
        constexpr auto aVal = 64531214;
        constexpr auto bVal = 451134534531214;
        struct B
        {
            uint64_t a{ static_cast<uint64_t>(aVal) };
            uint64_t b{ static_cast<uint64_t>(bVal) };
        } c;

        constexpr auto cba = hash(B{});


        constexpr HashValue prime{ 0x100000001b3ull };
        constexpr HashValue hashSeed = 0xcbf29ce484222325ull;

        constexpr auto aHashVal = (hashSeed ^ aVal) * prime;
        constexpr auto bHashVal = (hashSeed ^ bVal) * prime;

        constexpr auto tmpVal = (hashSeed ^ aHashVal) * prime;
        constexpr auto resultVal = (tmpVal ^ bHashVal) * prime;

        EXPECT_EQ(cba, resultVal);
    }
    TEST(Hashtests, SpanHash) {
        constexpr auto aVal = 64531214;
        constexpr auto bVal = 451134534531214;
        struct B
        {
            uint64_t a{ static_cast<uint64_t>(aVal) };
            uint64_t b{ static_cast<uint64_t>(bVal) };
        } c;

        std::array g{ B{}, B{}, B{}, B{} ,B{},B{},B{},B{},B{},B{} ,B{} ,B{} ,B{} ,B{} ,B{} ,B{} ,B{} ,B{} ,B{} ,B{} ,B{} ,B{} ,B{} ,B{} ,B{} ,B{} ,B{} ,B{} ,B{} };

        auto cba = hash(std::span{g} );


        EXPECT_EQ(cba, 14695981039346656037);
    }
    TEST(Hashtests, ByteHash) {
        std::vector<std::byte> data;

        constexpr auto testSize = 1 << 16;
        for(size_t i = 0; i < testSize; ++i)
        {
            data.push_back(static_cast<std::byte>(rand()% 255));
        }

        auto cba = hash(std::span{ data });


        EXPECT_EQ(cba, 13931541190864799959);
    }
}