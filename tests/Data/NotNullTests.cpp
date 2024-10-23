import NYANData;

#include <gtest/gtest.h>
#include <array>
#include <span>
#include <random>

namespace nyan
{
    TEST(NotNullTests, constructAndDereference) {
        int v = 0;
        nyan::NotNull<int*> a{ &v };

        *a = 2;

        nyan::NotNull<const int*> b{ &v };

        EXPECT_EQ(*b, 2);

        //b = nullptr; should not compile;
        //*b = 3; should not compile;
        int c = 3;
        b = &c;
        EXPECT_EQ(*b, 3);
    }
    TEST(NotNullTests, ArrowOp) {
        struct A {
            int c;
        } v;
        nyan::NotNull<A*> a{ &v };

        a->c = 3;

        EXPECT_EQ(v.c, 3);
    }    
    int test_fun(int* ptr) { return *ptr; };
    TEST(NotNullTests, ConversionOp) {
        int v = 34;
        nyan::NotNull<int*> a{ &v };

        EXPECT_EQ(test_fun(a), v);

    }
}