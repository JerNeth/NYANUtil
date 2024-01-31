import NYANData;

#include <gtest/gtest.h>

namespace nyan::util::data
{
    TEST(BitsetTests, setAndTest) {
        enum class Test : uint32_t {
            A,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,
            AA,
            AB,
            AC,
            AD,
            AE,
            AF,
            AG,
            AH,
            AI,
            AK,
            AL,
            AM,
            AN,
            AO,
            AP,
            AQ,
            AR,
            AS,
            AT,
            AU,
            AV,
            AW,
            AX,
            AY,
            AZ,
            BA,
            BB,
            BC,
            BD,
            BE,
            BF,
            BG,
            BH,
            BI,
            BK,
            BL,
            BM,
            BN,
            BO,
            BP,
            BQ,
            BR,
            BS,
            BT,
            BU,
            BV,
            BW,
            BX,
            BY,
            BZ,
            Size
        };
        using enum Test;
        bitset<static_cast<size_t>(Test::Size), Test> bitset;
        EXPECT_FALSE(bitset.test(A));
        EXPECT_FALSE(bitset.test(B));
        EXPECT_FALSE(bitset.test(C));
        EXPECT_FALSE(bitset.test(B));
        EXPECT_FALSE(bitset.test(D));
        EXPECT_FALSE(bitset.test(E));
        EXPECT_FALSE(bitset.test(F));
        EXPECT_FALSE(bitset.test(H));
        //EXPECT_FALSE(bitset.test(Size)); Will fail assertion

        bitset.set(A);
        EXPECT_TRUE(bitset.test(A));
        EXPECT_FALSE(bitset.test(B));
        EXPECT_FALSE(bitset.test(C));
        EXPECT_FALSE(bitset.test(B));
        EXPECT_FALSE(bitset.test(D));
        EXPECT_FALSE(bitset.test(E));
        EXPECT_FALSE(bitset.test(F));
        EXPECT_FALSE(bitset.test(H));

        for (auto i = static_cast<uint32_t>(A); i < static_cast<uint32_t>(Test::Size); ++i)
        {
            bitset.set(static_cast<Test>(i));
        }

        for (auto i = static_cast<uint32_t>(A); i < static_cast<uint32_t>(Test::Size); ++i)
        {
            EXPECT_TRUE(bitset.test(static_cast<Test>(i)));
            bitset.reset(static_cast<Test>(i));
            EXPECT_FALSE(bitset.test(static_cast<Test>(i)));
        }

        for (auto i = static_cast<uint32_t>(A); i < static_cast<uint32_t>(Test::Size); ++i)
        {
            EXPECT_FALSE(bitset.test(static_cast<Test>(i)));
        }
        bitset.set(C);
        bitset.set(BZ);
        for (auto i = static_cast<uint32_t>(A); i < static_cast<uint32_t>(Test::Size); ++i)
        {
            if (static_cast<Test>(i) != C && static_cast<Test>(i) != BZ)
                EXPECT_FALSE(bitset.test(static_cast<Test>(i)));
        }
        EXPECT_TRUE(bitset.test(static_cast<Test>(C)));
        EXPECT_TRUE(bitset.test(static_cast<Test>(BZ)));
    }
    TEST(BitsetTests, getAndClear) {
        enum class Test : uint32_t {
            A,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,
            AA,
            AB,
            AC,
            AD,
            AE,
            AF,
            AG,
            AH,
            AI,
            AK,
            AL,
            AM,
            AN,
            AO,
            AP,
            AQ,
            AR,
            AS,
            AT,
            AU,
            AV,
            AW,
            AX,
            AY,
            AZ,
            BA,
            BB,
            BC,
            BD,
            BE,
            BF,
            BG,
            BH,
            BI,
            BK,
            BL,
            BM,
            BN,
            BO,
            BP,
            BQ,
            BR,
            BS,
            BT,
            BU,
            BV,
            BW,
            BX,
            BY,
            BZ,
            Size
        };
        using enum Test;
        bitset<static_cast<size_t>(Test::Size), Test> bitset;

        bitset.set(A);
        EXPECT_TRUE(bitset.test(A));
        EXPECT_FALSE(bitset.test(B));
        EXPECT_FALSE(bitset.test(C));
        auto r = bitset.get_and_clear(Test::A);

        EXPECT_TRUE(r.test(A));
        EXPECT_FALSE(r.test(B));
        EXPECT_FALSE(r.test(C));

        EXPECT_FALSE(bitset.test(A));
        EXPECT_FALSE(bitset.test(B));
        EXPECT_FALSE(bitset.test(C));
    }

    TEST(BitsetTests, anyOf) {
        enum class Test : uint32_t {
            A,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,
            AA,
            AB,
            AC,
            AD,
            AE,
            AF,
            AG,
            AH,
            AI,
            AK,
            AL,
            AM,
            AN,
            AO,
            AP,
            AQ,
            AR,
            AS,
            AT,
            AU,
            AV,
            AW,
            AX,
            AY,
            AZ,
            BA,
            BB,
            BC,
            BD,
            BE,
            BF,
            BG,
            BH,
            BI,
            BK,
            BL,
            BM,
            BN,
            BO,
            BP,
            BQ,
            BR,
            BS,
            BT,
            BU,
            BV,
            BW,
            BX,
            BY,
            BZ,
            Size
        };
        using enum Test;
        bitset<static_cast<size_t>(Test::Size), Test> bitset;

        bitset.set(H);

        bitset.set(B);
        bitset.set(C);
        EXPECT_TRUE(bitset.any_of(B, C));

        bitset.reset(B);
        EXPECT_TRUE(bitset.any_of(B, C));

        bitset.reset(C);
        EXPECT_FALSE(bitset.any_of(B, C));

        bitset.set(C);
        EXPECT_TRUE(bitset.any_of(B, C));
    }
    TEST(BitsetTests, operators) {
        enum class Test : uint32_t {
            A,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,
            AA,
            AB,
            AC,
            AD,
            AE,
            AF,
            AG,
            AH,
            AI,
            AK,
            AL,
            AM,
            AN,
            AO,
            AP,
            AQ,
            AR,
            AS,
            AT,
            AU,
            AV,
            AW,
            AX,
            AY,
            AZ,
            BA,
            BB,
            BC,
            BD,
            BE,
            BF,
            BG,
            BH,
            BI,
            BK,
            BL,
            BM,
            BN,
            BO,
            BP,
            BQ,
            BR,
            BS,
            BT,
            BU,
            BV,
            BW,
            BX,
            BY,
            BZ,
            Size
        };
        using enum Test;
        bitset<static_cast<size_t>(Test::Size), Test> bitsetA;
        bitset<static_cast<size_t>(Test::Size), Test> bitsetB;

        bitsetA.set(A);
        bitsetB.set(B);

        EXPECT_TRUE(bitsetA);
        EXPECT_TRUE(bitsetB);
        EXPECT_TRUE(bitsetA | bitsetB);
        bitsetA.reset(A);
        EXPECT_FALSE(bitsetA);
        EXPECT_TRUE(bitsetA | bitsetB);

        bitsetB.reset(B);
        EXPECT_FALSE(bitsetB);
        EXPECT_FALSE(bitsetA | bitsetB);


        bitsetA.set(A);
        bitsetB.set(B);
        EXPECT_TRUE(bitsetA);
        EXPECT_TRUE(bitsetB);
        EXPECT_FALSE(bitsetA & bitsetB);
        bitsetA.set(B);
        EXPECT_TRUE(bitsetA & bitsetB);

    }
}