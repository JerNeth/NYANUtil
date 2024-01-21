import NYANMath;

#include <gtest/gtest.h>
#include <random>
#include <array>

namespace Math
{

    TEST(Matrices, ViewMatrix) {
        Math::vec3 a({ 0, 0, 1 }), b({ 0, 0, 1 }), c({ 0, 0, 1.001f }), d({ 0, 0, -1.0f }), e({ 0, 0, -1.001f });

        EXPECT_TRUE(close(a, a));
        EXPECT_TRUE(close(a, b));
        EXPECT_FALSE(close(a, c));
        EXPECT_TRUE(close(a, c, 0.01f));
        EXPECT_TRUE(close(d, d));
        EXPECT_FALSE(close(d, e));
        EXPECT_TRUE(close(d, e, 0.01f));
    }
    TEST(Matrices, Basic) {
        mat44 t;
        //EXPECT_EQ(a.dot(b), 0);
    }
    TEST(Matrices, MatrixVectorMultiplication) {
        mat33 test({ 2, 0, 0,  0, 1, 0, 0, 0, 1 });
        vec3 testv({ 1,1,1 });

        EXPECT_TRUE(close(test * testv, vec3({ 2,1,1 })));

        mat33 test2({ 2, 2, 2,  0, 0, 0, 0, 0, 0 });
        vec3 testv2({ 1,1,1 });

        EXPECT_TRUE(close(test2 * testv2, vec3({ 6,0,0 })));

        mat44 t44 = mat44::identity();
        mat33 t33 = mat33::identity();
        static std::default_random_engine rng;

        std::uniform_real_distribution<float> dist_vec(-100000.0, 100000.0);

        for (int i = 0; i < 10000; i++) {
            vec4 a({ dist_vec(rng), dist_vec(rng), dist_vec(rng), dist_vec(rng) });
            vec4 result1 = t44 * a;

            vec3 b({ dist_vec(rng), dist_vec(rng), dist_vec(rng) });
            vec3 result2 = t33 * b;

            EXPECT_TRUE(close(a, result1, 0.0001f)) << a.convert_to_string() << " not close to " << result1.convert_to_string() << " at: i=" << i;
            EXPECT_TRUE(close(b, result2, 0.0001f)) << b.convert_to_string() << " not close to " << result2.convert_to_string() << " at: i=" << i;
        }
        //EXPECT_EQ(a.dot(b), 0);
    }
    TEST(Matrices, transpose) {

        static std::default_random_engine rng;

        std::uniform_real_distribution<float> dist_vec(-100000.0, 100000.0);

        for (int i = 0; i < 10000; i++) {
            mat44 t({ dist_vec(rng), dist_vec(rng) , dist_vec(rng) ,dist_vec(rng),
                dist_vec(rng), dist_vec(rng) , dist_vec(rng) ,dist_vec(rng),
                dist_vec(rng), dist_vec(rng) , dist_vec(rng) ,dist_vec(rng),
                dist_vec(rng), dist_vec(rng) , dist_vec(rng) ,dist_vec(rng), });
            mat44 t_t = t.transpose();
            EXPECT_TRUE(close(t, t.transpose().transpose()));
            EXPECT_TRUE(close(t, t_t.transposed()));
        }
        //EXPECT_EQ(a.dot(b), 0);
    }
    TEST(Matrices, inverse) {

        static std::default_random_engine rng;

        std::uniform_real_distribution<float> dist_vec(-100000.0, 100000.0);
        auto counter = 0;
        for (int i = 0; i < 10000; i++) {
            mat44 t({ dist_vec(rng), dist_vec(rng) , dist_vec(rng) ,dist_vec(rng),
                dist_vec(rng), dist_vec(rng) , dist_vec(rng) ,dist_vec(rng),
                dist_vec(rng), dist_vec(rng) , dist_vec(rng) ,dist_vec(rng),
                dist_vec(rng), dist_vec(rng) , dist_vec(rng) ,dist_vec(rng), });
            float determinante;
            mat44 t_t;
            if (!t.inverse(t_t)) {
                counter++;
                continue;
            }
            EXPECT_TRUE(close(mat44::identity(), t * t_t, 1e-2f)) << t.convert_to_string() << "\n" << t_t.convert_to_string() << "\n" << (t * t_t).convert_to_string() << " should be:\n" << mat44::identity().convert_to_string() << "\n";
            EXPECT_TRUE(close(mat44::identity(), t_t * t, 1e-2f));
        }
        //std::cout << "Non invertible matrices: " << counter << "\n";
        //EXPECT_EQ(a.dot(b), 0);
    }
    TEST(Matrices, inverse2) {


        mat44 t(1.f, 1.f, 1.f, -1.f,
            1.f, 1.f, -1.f, 1.f,
            1.f, -1.f, 1.f, 1.f,
            -1.f, 1.f, 1.f, 1.f);
        mat44 t_t;
        if (!t.inverse(t_t)) {
        }
        EXPECT_TRUE(close(mat44::identity(), t * t_t)) << t.convert_to_string() << "\n" << t_t.convert_to_string() << "\n" << (t * t_t).convert_to_string() << " should be:\n" << mat44::identity().convert_to_string() << "\n";
        EXPECT_TRUE(close(mat44::identity(), t_t * t));

        //EXPECT_EQ(a.dot(b), 0);
    }
    TEST(Matrices, inverseRotation) {

        static std::default_random_engine rng;
        std::uniform_real_distribution<float> dist_rot(0.0, 360.0);
        std::uniform_real_distribution<float> dist_rot_half(0.0, 180.0);
        std::uniform_real_distribution<float> dist_pos(-100.0, 100.0);
        int counter1_2 = 0;
        int counter1_1 = 0;
        for (int i = 0; i < 100000; i++) {
            vec3 angles({ dist_rot(rng), dist_rot_half(rng), dist_rot(rng) });
            vec3 pos({ dist_pos(rng), dist_pos(rng) ,dist_pos(rng) });
            auto mat = mat44::affine_transformation_matrix(angles, pos);
            mat44 matInverse;
            if (!mat.inverse(matInverse)) {
                counter1_1++;
                continue;
            }
            auto rotMat = mat33::rotation_matrix(angles).transpose();
            mat44 mat2 = mat44(rotMat);
            mat2(3, 3) = 1;
            mat2.set_col(rotMat * -pos, 3ull);
            auto inv2 = mat.inverse_affine_transformation_matrix();
            EXPECT_TRUE(close(mat2, matInverse, 1e-3f)) << matInverse.convert_to_string() << " should be:\n" << mat2.convert_to_string() << "\n";
            EXPECT_TRUE(close(mat2, inv2, 1e-3f)) << inv2.convert_to_string() << " should be:\n" << mat2.convert_to_string() << "\n";

        }
        for (int i = 0; i < 100000; i++) {
            vec3 angles({ dist_rot(rng), dist_rot_half(rng), dist_rot(rng) });
            vec3 pos({ dist_pos(rng), dist_pos(rng) ,dist_pos(rng) });
            auto mat = mat34::affine_transformation_matrix(angles, pos);
            auto rotMat = mat33::rotation_matrix(angles).transpose();
            mat34 mat2 = mat34(rotMat);
            mat2.set_col(rotMat * -pos, 3ull);
            const auto inv2 = mat.inverse_affine_transformation_matrix();
            EXPECT_TRUE(close(mat2, static_cast<mat34>(inv2), 1e-3f)) << inv2.convert_to_string() << " should be:\n" << mat2.convert_to_string() << "\n";

        }
        EXPECT_EQ(counter1_1, 0) << counter1_1 << " trivially inversible matrices should have been inverted\n";
        EXPECT_EQ(counter1_2, 0);

        //EXPECT_EQ(a.dot(b), 0);
    }
    //TEST(Matrices, rowMajor) {

    //    Math::Mat<float, 3, 3, false> a(1, 1, 1,
    //        0, 0, 0,
    //        0, 0, 0
    //    );
    //    std::array<float, 9> b{ 1, 1, 1, 0, 0,0 ,0,0 ,0 };
    //    bool res = true;
    //    for (size_t i = 0; i < b.size(); i++) {
    //        res &= b[i] == static_cast<float>(a[i]);
    //    }

    //    EXPECT_TRUE(res);
    //    //EXPECT_EQ(a.dot(b), 0);
    //}
    //TEST(Matrices, colMajor) {

    //    Math::Mat<float, 3, 3, true> a(1, 1, 1,
    //        0, 0, 0,
    //        0, 0, 0
    //    );
    //    std::array<float, 9> b{ 1, 0, 0, 1, 0,0 ,1,0 ,0 };
    //    bool res = true;
    //    for (size_t i = 0; i < b.size(); i++) {
    //        res &= b[i] == a[i];
    //    }

    //    EXPECT_TRUE(res);
    //    //EXPECT_EQ(a.dot(b), 0);
    //}
    TEST(Matrices, perspectiveInverseDepth) {
        auto near = 0.1f;
        auto far = 10000.f;
        auto fov = 90.f;
        auto aspect = 16.f / 9.f;
        auto mat = mat44::perspectiveInverseDepthFovXLH(near, far, fov, aspect);

        {
            auto nearVec = vec4(0, 0, -near, 1);
            auto farVec = vec4(0, 0, -far, 1);
            auto result0 = vec4(0, 0, 1, 1);
            auto result1 = vec4(0, 0, 0, 1);
            nearVec = mat * nearVec;
            nearVec /= nearVec.w();
            farVec = mat * farVec;
            farVec /= farVec.w();
            EXPECT_EQ(nearVec, result0);
            EXPECT_EQ(farVec, result1);
        }
        {
            auto nearVecLeft = vec4(-tan(fov * 0.5 * Math::deg_to_rad) * near, 0, -near, 1);
            auto nearVecRight = vec4(tan(fov * 0.5 * Math::deg_to_rad) * near, 0, -near, 1);
            auto result0 = vec4(-1, 0, 1, 1);
            auto result1 = vec4(1, 0, 1, 1);
            nearVecLeft = mat * nearVecLeft;
            nearVecLeft /= nearVecLeft.w();
            nearVecRight = mat * nearVecRight;
            nearVecRight /= nearVecRight.w();
            EXPECT_EQ(nearVecLeft, result0);
            EXPECT_EQ(nearVecRight, result1);
        }
        {
            auto nearVecTop = vec4(0, tan(fov * 0.5 * Math::deg_to_rad) * near / aspect, -near, 1);
            auto nearVecBot = vec4(0, -tan(fov * 0.5 * Math::deg_to_rad) * near / aspect, -near, 1);
            auto result0 = vec4(0, -1, 1, 1);
            auto result1 = vec4(0, 1, 1, 1);
            nearVecTop = mat * nearVecTop;
            nearVecTop /= nearVecTop.w();
            nearVecBot = mat * nearVecBot;
            nearVecBot /= nearVecBot.w();
            EXPECT_EQ(nearVecTop, result0);
            EXPECT_EQ(nearVecBot, result1);
        }
    }
    TEST(Matrices, perspectiveFov) {
        auto near = 0.1f;
        auto far = 10000.f;
        auto fov = 90.f;
        auto aspect = 16.f / 9.f;
        auto mat = mat44::perspectiveFovXLH(near, far, fov, aspect);

        {
            auto nearVec = vec4(0, 0, -near, 1);
            auto farVec = vec4(0, 0, -far, 1);
            auto result0 = vec4(0, 0, 0, 1);
            auto result1 = vec4(0, 0, 1, 1);
            nearVec = mat * nearVec;
            nearVec /= nearVec.w();
            farVec = mat * farVec;
            farVec /= farVec.w();
            EXPECT_TRUE(close(nearVec, result0));
            EXPECT_TRUE(close(farVec, result1));
        }
        {
            auto nearVecLeft = vec4(-tan(fov * 0.5 * Math::deg_to_rad) * near, 0, -near, 1);
            auto nearVecRight = vec4(tan(fov * 0.5 * Math::deg_to_rad) * near, 0, -near, 1);
            auto result0 = vec4(-1, 0, 0, 1);
            auto result1 = vec4(1, 0, 0, 1);
            nearVecLeft = mat * nearVecLeft;
            nearVecLeft /= nearVecLeft.w();
            nearVecRight = mat * nearVecRight;
            nearVecRight /= nearVecRight.w();
            EXPECT_TRUE(close(nearVecLeft, result0));
            EXPECT_TRUE(close(nearVecRight, result1));
        }
        {
            auto nearVecTop = vec4(0, tan(fov * 0.5 * Math::deg_to_rad) * near / aspect, -near, 1);
            auto nearVecBot = vec4(0, -tan(fov * 0.5 * Math::deg_to_rad) * near / aspect, -near, 1);
            auto result0 = vec4(0, -1, 0, 1);
            auto result1 = vec4(0, 1, 0, 1);
            nearVecTop = mat * nearVecTop;
            nearVecTop /= nearVecTop.w();
            nearVecBot = mat * nearVecBot;
            nearVecBot /= nearVecBot.w();
            EXPECT_TRUE(close(nearVecTop, result0));
            EXPECT_TRUE(close(nearVecBot, result1));
        }
    }
    TEST(Matrices, multiplication) {

        mat33 a({ 1, 0, 0,
                 0, 1, 0,
                 0, 1, 0 });
        mat33 b({ 1, 0, 1,
                 0, 1, 0,
                 0, 0, 0 });
        mat33 c({ 1, 0, 1,
                 0, 1, 0,
                 0, 1, 0 });
        auto res = a * b;
        EXPECT_TRUE(close(res, c)) << res.convert_to_string() << " not equal to:\n" << c.convert_to_string() << "\n";

        {
            mat33 a({ 1, 0, 0,
                     0, 1, 0,
                     0, 0, 1 });
            auto res = a * a;
            EXPECT_TRUE(close(res, a)) << res.convert_to_string() << " not equal to:\n" << a.convert_to_string() << "\n";
        }
        {
            mat33 a({ 1, 0, 0,
                     0, 1, 0,
                     0, 0, 1 });
            mat33 b({ 1, 1, 5,
                     3, 1, 2,
                     4, 2, 1 });
            auto res = a * b;
            EXPECT_TRUE(close(res, b)) << res.convert_to_string() << " not equal to:\n" << b.convert_to_string() << "\n";
        }
        mat33 d({ 2, 0, 2,
                 0, 2, 0,
                 0, 2, 0 });
        mat33 e({ 1, 0, 1,
                 0, 1, 0,
                 1, 0, 0 });
        mat33 f({ 4, 0, 2,
                 0, 2, 0,
                 0, 2, 0 });
        mat33 g({ 2, 2, 2,
                 0, 2, 0,
                 2, 0, 2 });
        res = d * e;
        EXPECT_TRUE(close(res, f)) << res.convert_to_string() << " not equal to:\n" << f.convert_to_string() << "\n";
        res = e * d;
        EXPECT_TRUE(close(res, g)) << res.convert_to_string() << " not equal to:\n" << g.convert_to_string() << "\n";
        {

            mat34 d({ 2, 0, 2, 0,
                     0, 2, 0, 0,
                     0, 2, 0, 0 });
            mat43 e({ 1, 0, 1,
                     0, 1, 0,
                     1, 0, 0,
                     0, 0, 0 });
            mat33 r({ 4, 0, 2,
                     0, 2, 0,
                     0, 2, 0 });
            mat44 r2({ 2, 2, 2, 0,
                     0, 2, 0, 0,
                     2, 0, 2, 0,
                     0, 0, 0, 0 });
            auto res = d * e;
            auto res2 = e * d;
            EXPECT_TRUE(close(res, r)) << res.convert_to_string() << " should be:\n" << r.convert_to_string() << "\n";
            EXPECT_TRUE(close(res2, r2)) << res2.convert_to_string() << " should be:\n" << r2.convert_to_string() << "\n";
        }
        {

            Mat<float, 2, 4> d({ 2, 0, 2, 0,
                                0, 2, 0, 0 });
            Mat<float, 4, 2> e({ 1, 0,
                                0, 1,
                                1, 0,
                                0, 0, });
            mat22 r(4, 0,
                0, 2);
            mat44 r2({ 2, 0, 2, 0,
                     0, 2, 0, 0,
                     2, 0, 2, 0,
                     0, 0, 0, 0 });
            auto res = d * e;
            auto res2 = e * d;
            EXPECT_TRUE(close(res, r)) << res.convert_to_string() << " should be:\n" << r.convert_to_string() << "\n";
            EXPECT_TRUE(close(res2, r2)) << res2.convert_to_string() << " should be:\n" << r2.convert_to_string() << "\n";
        }
        {

            Mat<float, 2, 4> d({ 2, 0, 2, 0,
                                0, 2, 0, 0 });
            Mat<float, 4, 3> e({ 1, 0, 1,
                                0, 1, 1,
                                1, 0, 3,
                                0, 0, 2 });
            Mat<float, 2, 3> r(4, 0, 8,
                0, 2, 2);
            auto res = d * e;
            EXPECT_TRUE(close(res, r)) << res.convert_to_string() << " should be:\n" << r.convert_to_string() << "\n";
        }
    }
    TEST(Matrices, rotationMatrices) {
        // TODO: This Test is flawed currently
        static std::default_random_engine rng;

        std::uniform_real_distribution<float> dist_rot(0.0, 360.0);
        std::uniform_real_distribution<float> dist_rot_half(0.0, 180.0);
        std::uniform_real_distribution<float> dist_pos(-100.0, 100.0);
        int counter1_2 = 0;
        for (int i = 0; i < 100000; i++) {
            vec3 angles({ dist_rot(rng), dist_rot_half(rng), dist_rot(rng) });
            double yaw = angles[2], pitch = angles[1], roll = angles[0];
            float cy = (float)cos(yaw * deg_to_rad);
            float sy = (float)sin(yaw * deg_to_rad);
            float cp = (float)cos(pitch * deg_to_rad);
            float sp = (float)sin(pitch * deg_to_rad);
            float cr = (float)cos(roll * deg_to_rad);
            float sr = (float)sin(roll * deg_to_rad);
            mat33 roll_m(1, 0, 0,
                0, cr, -sr,
                0, sr, cr);
            mat33  pitch_m(cp, 0, sp,
                0, 1, 0,
                -sp, 0, cp);
            mat33  yaw_m(cy, -sy, 0,
                sy, cy, 0,
                0, 0, 1);
            mat33 rot = (yaw_m * pitch_m) * roll_m;

            vec3 pos({ dist_pos(rng), dist_pos(rng) ,dist_pos(rng) });
            vec3 result1 = mat33::rotation_matrix(angles) * pos;
            // vec3 result2 = rot * pos;
            vec3 result2 = yaw_m * (pitch_m * (roll_m * pos));

            if (!close(result1, result2, 0.01f))
                counter1_2++;
        }
        EXPECT_EQ(counter1_2, 0);

    }
    TEST(Matrices, X2) {
        std::vector input{ 1.0000000221841605 ,
                            0.0 ,
                            0.0,
                            0.0,
                            0.0 ,
                            0.99619472166546763 ,
                            0.087155727833476249,
                            0.0,
                            0.0,
                            -0.087155727833476249,
                            0.99619472166546763,
                            0.0,
                            -5.0000000000000000,
                            0.0,
                            0.0,
                            1.0 };
        Math::vec3 expected(5, 0, 0);

        Math::mat44 mat{ 1.0000000221841605 ,
                            0.0 ,
                            0.0,
                            0.0,
                            0.0 ,
                            0.99619472166546763 ,
                            0.087155727833476249,
                            0.0,
                            0.0,
                            -0.087155727833476249,
                            0.99619472166546763,
                            0.0,
                            -5.0000000000000000,
                            0.0,
                            0.0,
                            1.0 };
        auto expectedMat = Math::Mat<float, 3, 3, true>::rotation_matrix(expected);
        Math::mat33 matSmall;
        Math::vec3 tmpX{ mat.col(0) };
        Math::vec3 tmpY{ mat.col(1) };
        Math::vec3 tmpZ{ mat.col(2) };
        matSmall.set_col(tmpX, 0);
        matSmall.set_col(tmpY, 1);
        matSmall.set_col(tmpZ, 2);
        std::array vals{
            Math::vec3(0, 0, 1),
            Math::vec3(0, 1, 0),
            Math::vec3(1, 0, 0)
        };
        Math::vec3 orientation = matSmall.euler();
        Math::quat quaternion{ matSmall };
        auto inferedMat = Math::Mat<float, 3, 3, true>::rotation_matrix(Math::vec3{ orientation });
        for (auto val : vals) {
            //  EXPECT_TRUE(close(matSmall * val, expectedMat * val)) << (matSmall * val).convert_to_string() << " not equal to " << (expectedMat * val).convert_to_string();
            //  EXPECT_TRUE(close(matSmall * val, quaternion * val)) << (matSmall * val).convert_to_string() << " not equal to " << (quaternion * val).convert_to_string();
            EXPECT_TRUE(close(matSmall * val, inferedMat * val)) << (matSmall * val).convert_to_string() << " not equal to " << (inferedMat * val).convert_to_string();
        }
    }
    TEST(Matrices, Y2) {
        std::vector input{ 0.97814764136552634 ,
                            0.0 ,
                            0.20791169731909154,
                            0.0,
                            0.0 ,
                            1.0000000410955230 ,
                            0.0,
                            0.0,
                            -0.20791169731909154,
                            0.0,
                            0.97814764136552634,
                            0.0,
                            0.0,
                            -5.0,
                            0.0,
                            1.0 };
        Math::vec3 expected(0, -12, 0);


        Math::mat44 mat{ 0.97814764136552634 ,
                            0.0 ,
                            0.20791169731909154,
                            0.0,
                            0.0 ,
                            1.0000000410955230 ,
                            0.0,
                            0.0,
                            -0.20791169731909154,
                            0.0,
                            0.97814764136552634,
                            0.0,
                            0.0,
                            -5.0,
                            0.0,
                            1.0 };
        auto expectedMat = Math::Mat<float, 3, 3, true>::rotation_matrix(expected);
        Math::mat33 matSmall;
        Math::vec3 tmpX{ mat.col(0) };
        Math::vec3 tmpY{ mat.col(1) };
        Math::vec3 tmpZ{ mat.col(2) };
        matSmall.set_col(tmpX, 0);
        matSmall.set_col(tmpY, 1);
        matSmall.set_col(tmpZ, 2);
        std::array vals{
            Math::vec3(0, 0, 1),
            Math::vec3(0, 1, 0),
            Math::vec3(1, 0, 0)
        };
        Math::vec3 orientation = matSmall.euler();
        Math::quat quaternion{ matSmall };
        auto inferedMat = Math::Mat<float, 3, 3, true>::rotation_matrix(Math::vec3{ orientation });
        for (auto val : vals) {
            // EXPECT_TRUE(close(matSmall * val, expectedMat * val)) << (matSmall * val).convert_to_string() << " not equal to " << (expectedMat * val).convert_to_string();
            // EXPECT_TRUE(close(matSmall * val, quaternion * val)) << (matSmall * val).convert_to_string() << " not equal to " << (quaternion * val).convert_to_string();
            EXPECT_TRUE(close(matSmall * val, inferedMat * val)) << (matSmall * val).convert_to_string() << " not equal to " << (inferedMat * val).convert_to_string();
        }
    }
    TEST(Matrices, Z2) {
        std::vector input{ 0.95630478637671223 ,
                            -0.29237168909633660 ,
                            0.0,
                            0.0,
                            0.29237168909633660 ,
                            0.95630478637671223 ,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            1.0000000245160268,
                            0.0,
                            0.0,
                            0.0,
                            -5.0,
                            1.0 };
        Math::vec3 expected(0, 0, -17);

        Math::mat44 mat{ 0.95630478637671223 ,
                            -0.29237168909633660 ,
                            0.0,
                            0.0,
                            0.29237168909633660 ,
                            0.95630478637671223 ,
                            0.0,
                            0.0,
                            0.0,
                            0.0,
                            1.0000000245160268,
                            0.0,
                            0.0,
                            0.0,
                            -5.0,
                            1.0 };
        auto expectedMat = Math::Mat<float, 3, 3, true>::rotation_matrix(expected);
        Math::mat33 matSmall;
        Math::vec3 tmpX{ mat.col(0) };
        Math::vec3 tmpY{ mat.col(1) };
        Math::vec3 tmpZ{ mat.col(2) };
        matSmall.set_col(tmpX, 0);
        matSmall.set_col(tmpY, 1);
        matSmall.set_col(tmpZ, 2);
        std::array vals{
            Math::vec3(0, 0, 1),
            Math::vec3(0, 1, 0),
            Math::vec3(1, 0, 0)
        };
        Math::vec3 orientation = matSmall.euler();
        Math::quat quaternion{ matSmall };
        auto inferedMat = Math::Mat<float, 3, 3, false>::rotation_matrix(Math::vec3{ orientation });
        for (auto val : vals) {
            // EXPECT_TRUE(close(matSmall * val, expectedMat * val)) << (matSmall * val).convert_to_string() << " not equal to " << (expectedMat * val).convert_to_string();
             //EXPECT_TRUE(close(matSmall * val, quaternion * val)) << (matSmall * val).convert_to_string() << " not equal to " << (quaternion * val).convert_to_string();
            EXPECT_TRUE(close(matSmall * val, inferedMat * val)) << (matSmall * val).convert_to_string() << " not equal to " << (inferedMat * val).convert_to_string();
        }
    }
}