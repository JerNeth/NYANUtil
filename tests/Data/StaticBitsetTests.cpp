#include <gtest/gtest.h>
#include <array>

import NYANData;


namespace nyan
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
        bitset<static_cast<size_t>(Test::Size), Test> bset;
        EXPECT_FALSE(bset.test(A));
        EXPECT_FALSE(bset.test(B));
        EXPECT_FALSE(bset.test(C));
        EXPECT_FALSE(bset.test(B));
        EXPECT_FALSE(bset.test(D));
        EXPECT_FALSE(bset.test(E));
        EXPECT_FALSE(bset.test(F));
        EXPECT_FALSE(bset.test(H));
        //EXPECT_FALSE(bitset.test(Size)); Will fail assertion

        bset.set(A);
        EXPECT_TRUE(bset.test(A));
        EXPECT_FALSE(bset.test(B));
        EXPECT_FALSE(bset.test(C));
        EXPECT_FALSE(bset.test(B));
        EXPECT_FALSE(bset.test(D));
        EXPECT_FALSE(bset.test(E));
        EXPECT_FALSE(bset.test(F));
        EXPECT_FALSE(bset.test(H));

        for (auto i = static_cast<uint32_t>(A); i < static_cast<uint32_t>(Test::Size); ++i)
        {
            bset.set(static_cast<Test>(i));
        }

        for (auto i = static_cast<uint32_t>(A); i < static_cast<uint32_t>(Test::Size); ++i)
        {
            EXPECT_TRUE(bset.test(static_cast<Test>(i)));
            bset.reset(static_cast<Test>(i));
            EXPECT_FALSE(bset.test(static_cast<Test>(i)));
        }

        for (auto i = static_cast<uint32_t>(A); i < static_cast<uint32_t>(Test::Size); ++i)
        {
            EXPECT_FALSE(bset.test(static_cast<Test>(i)));
        }
        bset.set(C);
        bset.set(BZ);
        for (auto i = static_cast<uint32_t>(A); i < static_cast<uint32_t>(Test::Size); ++i)
        {
            if (static_cast<Test>(i) != C && static_cast<Test>(i) != BZ)
                EXPECT_FALSE(bset.test(static_cast<Test>(i)));
        }
        EXPECT_TRUE(bset.test(static_cast<Test>(C)));
        EXPECT_TRUE(bset.test(static_cast<Test>(BZ)));

        bitset<static_cast<size_t>(Test::Size), Test> tval = ~bitset<static_cast<size_t>(Test::Size), Test>{};

        auto tmp = tval & bset;

        for (size_t a = 0; a < static_cast<size_t>(Test::Size); ++a)
            EXPECT_TRUE(tval.test(static_cast<Test>(a)));

        EXPECT_EQ(bset, tmp);


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
    TEST(BitsetTests, foreach) {
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
        bitsetA.set(L);
        bitsetA.set(M);
        bitsetA.set(T);
        bitsetA.set(Y);
        bitsetA.set(Z);
        bitsetA.set(AA);
        bitsetA.set(BS);
        bitsetA.set(BZ);

        bitsetA.for_each([&](Test t) {
                bitsetB.flip(t); //Doesn't ABA test, but at least tests against even amount of test executions
            });

        EXPECT_EQ(bitsetA, bitsetB);

        {
            bitset<static_cast<size_t>(Test::Size), Test> bitsetB;
            for (auto a : bitsetA)
                bitsetB.flip(a);

            EXPECT_EQ(bitsetA, bitsetB);
        }

        {
            bitset<static_cast<size_t>(Test::Size), Test> bitsetB;
            for (auto a : bitsetA)
                bitsetB.set(a);

            EXPECT_TRUE(bitsetB.test(A));
            EXPECT_TRUE(bitsetB.test(L));
            EXPECT_TRUE(bitsetB.test(M));
            EXPECT_TRUE(bitsetB.test(T));
            EXPECT_TRUE(bitsetB.test(Y));
            EXPECT_TRUE(bitsetB.test(Z));
            EXPECT_TRUE(bitsetB.test(AA));
            EXPECT_TRUE(bitsetB.test(BS));
            EXPECT_TRUE(bitsetB.test(BZ));
        }

        size_t counter{ 0 };

        bitsetA.for_each([&](Test t) {
                counter++;
            });
        EXPECT_EQ(counter, 9);

    }

    TEST(BitsetTests, iterators) {
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

        bitsetA.set(A);
        bitsetA.set(L);
        bitsetA.set(M);
        bitsetA.set(T);
        bitsetA.set(Y);
        bitsetA.set(Z);
        bitsetA.set(AA);
        bitsetA.set(BO);
        bitsetA.set(BP);
        bitsetA.set(BQ);
        bitsetA.set(BS);
        bitsetA.set(BZ);

        {
            bitset<static_cast<size_t>(Test::Size), Test> bitsetB;
            for (auto a : bitsetA)
                bitsetB.flip(a);

            EXPECT_EQ(bitsetA, bitsetB);
        }

        {
            bitset<static_cast<size_t>(Test::Size), Test> bitsetB;
            for (auto a : bitsetA)
                bitsetB.set(a);

            EXPECT_TRUE(bitsetB.test(A));
            EXPECT_TRUE(bitsetB.test(L));
            EXPECT_TRUE(bitsetB.test(M));
            EXPECT_TRUE(bitsetB.test(T));
            EXPECT_TRUE(bitsetB.test(Y));
            EXPECT_TRUE(bitsetB.test(Z));
            EXPECT_TRUE(bitsetB.test(AA));
            EXPECT_TRUE(bitsetB.test(BO));
            EXPECT_TRUE(bitsetB.test(BP));
            EXPECT_TRUE(bitsetB.test(BQ));
            EXPECT_TRUE(bitsetB.test(BS));
            EXPECT_TRUE(bitsetB.test(BZ));
        }


        size_t counter{ 0 };
        for (auto a : bitsetA)
            counter++;
        EXPECT_EQ(counter, 12);

    }
    TEST(BitsetTests, bench) {
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


        constexpr size_t iters = 10000;
        constexpr size_t innerIters = 100;
        {

            auto start = std::chrono::steady_clock::now();
            srand(0x12789432u);
            for (uint64_t i = 0; i < iters; i++) {
                bitset<static_cast<size_t>(Test::Size), Test> bitsetA;
                bitset<static_cast<size_t>(Test::Size), Test> bitsetB;

                for (auto j = 0; j < innerIters; ++j)
                {
                    bitsetA.flip(static_cast<Test>(rand() % static_cast<uint32_t>(Test::Size)));
                }
                for (auto a : bitsetA)
                    bitsetB.flip(a);

                EXPECT_EQ(bitsetA, bitsetB);
            }
            auto end = std::chrono::steady_clock::now();
            std::cout << "Iterators iterate took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "microseconds\n";
        }
        {

            auto start = std::chrono::steady_clock::now();
            srand(0x12789432u);
            for (uint64_t i = 0; i < iters; i++) {
                bitset<static_cast<size_t>(Test::Size), Test> bitsetA;
                bitset<static_cast<size_t>(Test::Size), Test> bitsetB;

                for (auto j = 0; j < innerIters; ++j)
                {
                    bitsetA.flip(static_cast<Test>(rand() % static_cast<uint32_t>(Test::Size)));
                }

                bitsetA.for_each([&](Test t) {
                    bitsetB.flip(t); //Doesn't ABA test, but at least tests against even amount of test executions
                    });

                EXPECT_EQ(bitsetA, bitsetB);
            }
            auto end = std::chrono::steady_clock::now();
            std::cout << "For_each iterate took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "microseconds\n";
        }
        {

            auto start = std::chrono::steady_clock::now();
            srand(0x12789432u);
            for (uint64_t i = 0; i < iters; i++) {
                std::array<bool, static_cast<size_t>(Test::Size)> bitsetA{ false };
                std::array<bool, static_cast<size_t>(Test::Size)> bitsetB{ false };

                for (auto j = 0; j < innerIters; ++j)
                {
                    bitsetA[static_cast<size_t>(rand() % static_cast<uint32_t>(Test::Size))] = !bitsetA[static_cast<size_t>(rand() % static_cast<uint32_t>(Test::Size))];
                }

                for (auto j = 0; j < bitsetA.size(); ++j) {
                    if(bitsetA[j])
                        bitsetB[j] = !bitsetB[j];
                }

                EXPECT_EQ(bitsetA, bitsetB);
            }
            auto end = std::chrono::steady_clock::now();
            std::cout << "Array iterate took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "microseconds\n";
        }
        {

            auto start = std::chrono::steady_clock::now();
            srand(0x12789432u);
            for (uint64_t i = 0; i < iters; i++) {
                bitset<static_cast<size_t>(Test::Size), Test> bitsetA;
                bitset<static_cast<size_t>(Test::Size), Test> bitsetB;

                for (auto j = 0; j < innerIters; ++j)
                {
                    bitsetA.flip(static_cast<Test>(rand() % static_cast<uint32_t>(Test::Size)));
                }
                for (auto a : bitsetA)
                    bitsetB.flip(a);

                EXPECT_EQ(bitsetA, bitsetB);
            }
            auto end = std::chrono::steady_clock::now();
            std::cout << "Iterators iterate took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "microseconds\n";
        }
        {

            auto start = std::chrono::steady_clock::now();
            srand(0x12789432u);
            for (uint64_t i = 0; i < iters; i++) {
                bitset<static_cast<size_t>(Test::Size), Test> bitsetA;
                bitset<static_cast<size_t>(Test::Size), Test> bitsetB;

                for (auto j = 0; j < innerIters; ++j)
                {
                    bitsetA.flip(static_cast<Test>(rand() % static_cast<uint32_t>(Test::Size)));
                }

                bitsetA.for_each([&](Test t) {
                    bitsetB.flip(t); //Doesn't ABA test, but at least tests against even amount of test executions
                    });

                EXPECT_EQ(bitsetA, bitsetB);
            }
            auto end = std::chrono::steady_clock::now();
            std::cout << "For_each iterate took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "microseconds\n";
        }
        {

            auto start = std::chrono::steady_clock::now();
            srand(0x12789432u);
            for (uint64_t i = 0; i < iters; i++) {
                std::array<bool, static_cast<size_t>(Test::Size)> bitsetA{ false };
                std::array<bool, static_cast<size_t>(Test::Size)> bitsetB{ false };

                for (auto j = 0; j < innerIters; ++j)
                {
                    bitsetA[static_cast<size_t>(rand() % static_cast<uint32_t>(Test::Size))] = !bitsetA[static_cast<size_t>(rand() % static_cast<uint32_t>(Test::Size))];
                }

                for (auto j = 0; j < bitsetA.size(); ++j) {
                    if (bitsetA[j])
                        bitsetB[j] = !bitsetB[j];
                }

                EXPECT_EQ(bitsetA, bitsetB);
            }
            auto end = std::chrono::steady_clock::now();
            std::cout << "Array iterate took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "microseconds\n";
        }
    }
    TEST(BitsetTests, hiddenBits) {
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

        for (size_t i = 0; i < static_cast<size_t>(Test::Size); ++i)
            bitsetA.set(static_cast<Test>(i));

        EXPECT_EQ(bitsetA, ~bitsetB);

        EXPECT_TRUE(bitsetA.has_all());
        EXPECT_TRUE(bitsetA.all_of(~bitsetB));
        EXPECT_TRUE((~bitsetB).all_of(bitsetA));
        EXPECT_TRUE(bitsetA.has_any());
        EXPECT_TRUE(bitsetA.any_of(~bitsetB));
        EXPECT_TRUE((~bitsetB).any_of(bitsetA));

        size_t counter{ 0 };
        for (auto a : ~bitsetB)
            counter++;
        EXPECT_EQ(counter, static_cast<size_t>(Test::Size));

        size_t counter2{ 0 };
        (~bitsetB).for_each([&counter2](auto a) {
                counter2++;
            });
        EXPECT_EQ(counter2, static_cast<size_t>(Test::Size));

        auto a = bitsetA.to_ulong();
        {
            (~bitsetB).for_each([&](auto a) {
                EXPECT_TRUE(bitsetA.test(a));
                });
            for (auto a : ~bitsetB)
                EXPECT_TRUE(bitsetA.test(a));

            bitsetA.for_each([&](auto a) {
                EXPECT_TRUE((~bitsetB).test(a));
                });
            for (auto a : bitsetA)
                EXPECT_TRUE((~bitsetB).test(a));
        }
        {
            bitset<static_cast<size_t>(Test::Size), Test> bitsetC;

            for (auto a : bitsetA)
                bitsetC.set(a);
            for (size_t i = 0; i < static_cast<size_t>(Test::Size); ++i)
                EXPECT_TRUE(bitsetC.test(static_cast<Test>(i)));
        }
        {
            bitset<static_cast<size_t>(Test::Size), Test> bitsetC;

            bitsetA.for_each([&](auto a) {
                bitsetC.set(a);
                });
            for (size_t i = 0; i < static_cast<size_t>(Test::Size); ++i)
                EXPECT_TRUE(bitsetC.test(static_cast<Test>(i)));
        }

        {
            bitset<static_cast<size_t>(Test::Size), Test> bitsetC;

            bitsetC = ~bitsetC;

            for (size_t i = 0; i < static_cast<size_t>(Test::Size); ++i)
                bitsetC.reset(static_cast<Test>(i));
            bitsetC.set(Test::BY);
            EXPECT_TRUE(bitsetC.only(Test::BY));
        }

    }
}