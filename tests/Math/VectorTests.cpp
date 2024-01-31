import NYANMath;

#include <gtest/gtest.h>
#include <random>
#include <array>

namespace nyan::util::math
{

    struct Vectors : public ::testing::Test {
        vec2 a2{ 0 };
        vec2 b2{ 1 };
        vec2 c2{ 2 };
        vec3 a3{ 0 };
        vec3 b3{ 1 };
        vec3 c3{ 2 };
        vec4 a4{ 0 };
        vec4 b4{ 1 };
        vec4 c4{ 2 };
        //static SetUpTestSuite()
        //For each test:
        //Vectors()
        //void SetUp();
        //--Test-Body--
        //void TearDown();
        //~Vectors()
        //--end for each
        //static TearDownTestSuite()
    };

    TEST_F(Vectors, Assignment) {
        vec4 test({ 0,0,0,0 });
        test.x() = 5;
        test.y() -= 4;
        test[2] = 3;
        test[3] = 4;
        EXPECT_EQ(test, vec4({ 5,-4,3,4 }));
    }
    TEST_F(Vectors, Addition) {

        vec2 a2(0), b2(1), c2(2);
        vec3 a3(0), b3(1), c3(2);
        vec4 a4(0), b4(1), c4(2);
        EXPECT_EQ(a2 + b2, b2);
        EXPECT_EQ(a3 + b3, b3);
        EXPECT_EQ(a4 + b4, b4);

        EXPECT_EQ(b2 + b2, c2);
        EXPECT_EQ(b3 + b3, c3);
        EXPECT_EQ(b4 + b4, c4);
    }
    TEST_F(Vectors, Subtraction) {

        EXPECT_EQ(b2 - a2, b2);
        EXPECT_EQ(b3 - a3, b3);
        EXPECT_EQ(b4 - a4, b4);

        EXPECT_EQ(b2 - b2, a2);
        EXPECT_EQ(b3 - b3, a3);
        EXPECT_EQ(b4 - b4, a4);
    }

    struct TestTuple {
        vec4 a;
        vec4 b;
        vec4 c;
    };
    struct VectorWithParams : public ::testing::TestWithParam<TestTuple> {
    };
    TEST_P(VectorWithParams, Subtraction) {

        auto [a, b, c] = GetParam();
        EXPECT_EQ(a - b, c);
    }
    const TestTuple tests[] = { TestTuple{vec4{2}, vec4{1}, vec4{1}} ,
                                 TestTuple{vec4{std::numeric_limits<float>::max()}, vec4{std::numeric_limits<float>::max()}, vec4{0}} };
    INSTANTIATE_TEST_SUITE_P(SimpleNumbers, VectorWithParams,
        ::testing::ValuesIn(tests)
    );
    TEST_F(Vectors, Multiplication) {
        vec2 a2(0), b2(1), c2(2);
        vec3 a3(0), b3(1), c3(2);
        vec4 a4(0), b4(1), c4(2);

        EXPECT_EQ(a2 * b2, a2);
        EXPECT_EQ(a3 * b3, a3);
        EXPECT_EQ(a4 * b4, a4);

        EXPECT_EQ(b2 * b2, b2);
        EXPECT_EQ(b3 * b3, b3);
        EXPECT_EQ(b4 * b4, b4);

        EXPECT_EQ(b2 * c2, c2);
        EXPECT_EQ(b3 * c3, c3);
        EXPECT_EQ(b4 * c4, c4);
    }
    TEST_F(Vectors, Division) {
        vec2 a2(0), b2(1), c2(2);
        vec3 a3(0), b3(1), c3(2);
        vec4 a4(0), b4(1), c4(2);

        EXPECT_EQ(c2 / 1, c2);
        EXPECT_EQ(c3 / 1, c3);
        EXPECT_EQ(c4 / 1, c4);

        EXPECT_EQ(b2 / 1, b2);
        EXPECT_EQ(b3 / 1, b3);
        EXPECT_EQ(b4 / 1, b4);

        EXPECT_EQ(c2 / 2, b2);
        EXPECT_EQ(c3 / 2, b3);
        EXPECT_EQ(c4 / 2, b4);
    }
    TEST_F(Vectors, Pow) {
        vec2 a2(0), b2(1), c2(2);
        vec3 a3(0), b3(1), c3(2);
        vec4 a4(0), b4(1), c4(2);

        EXPECT_EQ(b2 ^ 2, b2);
        EXPECT_EQ(b3 ^ 2, b3);
        EXPECT_EQ(b4 ^ 2, b4);

        EXPECT_EQ(b2 ^ 0, b2);
        EXPECT_EQ(b3 ^ 0, b3);
        EXPECT_EQ(b4 ^ 0, b4);

        EXPECT_EQ(c4 ^ c4, vec4(4));
        EXPECT_EQ(a2 ^ 0, b2);
        EXPECT_EQ(a3 ^ 0, b3);
        EXPECT_EQ(a4 ^ 0, b4);

        EXPECT_EQ(c2 ^ 0, b2);
        EXPECT_EQ(c3 ^ 0, b3);
        EXPECT_EQ(c4 ^ 0, b4);

        EXPECT_EQ(c2 ^ 1, c2);
        EXPECT_EQ(c3 ^ 1, c3);
        EXPECT_EQ(c4 ^ 1, c4);

        EXPECT_EQ(c2 ^ 2, vec2(4));
        EXPECT_EQ(c3 ^ 2, vec3(4));
        EXPECT_EQ(c4 ^ 2, vec4(4));
    }

    TEST_F(Vectors, Dot) {
        vec2 a2(0), b2(1), c2(2);
        vec3 a3(0), b3(1), c3(2);
        vec4 a4(0), b4(1), c4(2);

        EXPECT_EQ(dot(b2, b2), 2);
        EXPECT_EQ(dot(b3, b3), 3);
        EXPECT_EQ(dot(b4, b4), 4);

        EXPECT_EQ(dot(b2, c2), 4);
        EXPECT_EQ(dot(b3, c3), 6);
        EXPECT_EQ(dot(b4, c4), 8);

        EXPECT_EQ(dot(c2, c2), 8);
        EXPECT_EQ(dot(c3, c3), 12);
        EXPECT_EQ(dot(c4, c4), 16);

        EXPECT_EQ(b2.dot(b2), 2);
        EXPECT_EQ(b3.dot(b3), 3);
        EXPECT_EQ(b4.dot(b4), 4);

        EXPECT_EQ(b2.dot(c2), 4);
        EXPECT_EQ(b3.dot(c3), 6);
        EXPECT_EQ(b4.dot(c4), 8);

        EXPECT_EQ(c2.dot(c2), 8);
        EXPECT_EQ(c3.dot(c3), 12);
        EXPECT_EQ(c4.dot(c4), 16);
    }
    TEST_F(Vectors, Lerp) {
        vec2 a2(0), b2(1), c2(0.5);
        vec3 a3(0), b3(1), c3(0.5);
        vec4 a4(0), b4(1), c4(0.5);
        float t = 0.5f;
        EXPECT_EQ(lerp(a2, b2, t), c2);
        EXPECT_EQ(lerp(a3, b3, t), c3);
        EXPECT_EQ(lerp(a4, b4, t), c4);

    }
    TEST_F(Vectors, Broadcast) {
        vec2 a2(0), b2(1), c2(2);
        vec3 a3(0), b3(1), c3(2);
        vec4 a4(0), b4(1), c4(2);
        auto fun = [](float a) { return a * 2; };

        EXPECT_EQ(a2.apply_fun(fun), vec2(0));
        EXPECT_EQ(b4.apply_fun(fun), vec4(2));

        EXPECT_EQ(c4.apply_fun(fun), vec4(4));
    }

    TEST_F(Vectors, Cross) {
        vec3 a({ 0, 0, 1 }), b({ 1, 0, 0 }), c({ 0, 1, 0 });

        EXPECT_EQ(cross(a, b), c);
        EXPECT_EQ(cross(c, a), b);
        EXPECT_EQ(cross(b, c), a);

        EXPECT_EQ(cross(b, a), -c);
        EXPECT_EQ(cross(a, c), -b);
        EXPECT_EQ(cross(c, b), -a);
    }
    TEST_F(Vectors, Close) {
        vec3 a({ 0, 0, 1 }), b({ 0, 0, 1 }), c({ 0, 0, 1.001f }), d({ 0, 0, -1.0f }), e({ 0, 0, -1.001f });

        EXPECT_TRUE(close(a, a));
        EXPECT_TRUE(close(a, b));
        EXPECT_FALSE(close(a, c));
        EXPECT_TRUE(close(a, c, 0.01f));
        EXPECT_TRUE(close(d, d));
        EXPECT_FALSE(close(d, e));
        EXPECT_TRUE(close(d, e, 0.01f));
    }
    vec2 mix(vec2 x, vec2 y, vec2 a) {
        return x * (vec2(1.f) - a) + a * y;
    }
    vec2 step(float step, float a) {
        if (a < step)
            return vec2{ 0.f };
        else
            return vec2{ 1.f };
    }

    vec2 octahedral(vec3 n) {
        n *= 1.f / vec3(abs(n[0]), abs(n[1]), abs(n[2])).dot(vec3{ 1.f });
        if (n.z() < 0) {
        }
        vec2 tmp{ abs(n.y()), abs(n.x()) };
        tmp = 1.0f - tmp;
        if (n.x() < 0)
            tmp.x() *= -1;
        if (n.y() < 0)
            tmp.y() *= -1;
        //n.x() = tmp.x();
        //n.y() = tmp.y();

        //return n.xy();
        return mix(tmp, n.xy(), step(0.0f, n.z()));
    }
    vec3 deoctahedral(vec2 n) {
        vec3 res{ n[0], n[1], 1.f - abs(n[0]) - abs(n[1]) };
        //if (res[2] < 0) {
        //    vec2 tmp{ abs(res.y()), abs(res.x()) };
        //    tmp = 1.0f - tmp;
        //    if (n.x() < 0)
        //        tmp.x() *= -1;
        //    if (n.y() < 0)
        //        tmp.y() *= -1;
        //    res[0] = tmp[0];
        //    res[1] = tmp[1];
        //}
        vec2 tmp{ abs(res.y()), abs(res.x()) };
        tmp = 1.0f - tmp;
        if (n.x() < 0)
            tmp.x() *= -1;
        if (n.y() < 0)
            tmp.y() *= -1;
        //res[0] = tmp[0];
        //res[1] = tmp[1];
        auto tmp2 = mix(tmp, res.xy(), step(0.0f, res.z()));
        res[0] = tmp2[0];
        res[1] = tmp2[1];
        return res.normalized();
    }

    TEST_F(Vectors, Octahedral) {
        std::array as{ vec3{0, 0, 1}, vec3{0, 0, 1}, vec3{0, 0, 1.f}
        , vec3{0.16, 0.02f, -0.6f}
        , vec3{0, 0.01f, -0.99f}
        , vec3{1, 0.01f, 0.f}
        , vec3{0.7, 0.5f, -0.1f}
        , vec3{-0.4, -0.01f, -0.99f}
        , vec3{-0.5, 0.01f, -0.99f}
        , vec3{0.5, 0.4f, 0.2f}
        , vec3{0.2, 0.6f, 0.4f}
        , vec3{0.1, 0.2f, 0.6f} };
        for (auto& a : as)
            a.normalize();

        for (const auto& a : as)
            EXPECT_TRUE(close(a, deoctahedral(octahedral(a)), 1e-5)) << a.convert_to_string() << "->" << octahedral(a).convert_to_string() << "->" << deoctahedral(octahedral(a)).convert_to_string() << "\n";
    }
}