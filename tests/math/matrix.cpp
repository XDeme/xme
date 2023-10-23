#include <iostream>
#include <xme/math/glsl_mapping.hpp>
#include <xme/math/matrix.hpp>

namespace math = xme::math;

int test_access() {
    int errors = 0;
    math::Matrix<float, 2, 3> m{math::vec3(2, 1, 3), math::vec3(8, 7, 6)};
    static_assert(std::is_same_v<decltype(m.row(0)), math::vec2>);
    static_assert(std::is_same_v<decltype(m.column(0)), math::vec3>);
    static_assert(std::is_same_v<decltype(m[0]), math::vec3&>);
    {
        bool error = m.row(0) != math::vec2{2, 8} || m.row(1) != math::vec2{1, 7};
        error |= m.row(2) != math::vec2{3, 6};
        if (error) {
            std::cerr << "xme::Matrix::row error\n";
            ++errors;
        }
    }
    {
        bool error =
            m.column(0) != math::vec3{2, 1, 3} || m.column(1) != math::vec3{8, 7, 6};
        if (error) {
            std::cerr << "xme::Matrix::column error\n";
            ++errors;
        }
    }
    {
        math::Matrix<float, 2, 3> m1{1};
        bool error = m1[0] != math::vec3{1, 0, 0} || m1[1] != math::vec3{0, 1, 0};
        if (error) {
            std::cerr << "xme::Matrix::operator[] error\n";
            ++errors;
        }
    }
    return errors;
}

int test_operators() {
    int errors = 0;
    {
        math::Matrix<float, 2, 3> m{math::vec3{5}, math::vec3{-3}};
        auto r1 = m + 2;
        auto r2 = m + math::Matrix<float, 2, 3>{2};

        bool error = r1[0] != math::vec3{7} || r1[1] != math::vec3{-1};
        error |= r2[0] != math::vec3{7, 5, 5} || r2[1] != math::vec3{-3, -1, -3};
        if (error) {
            ++errors;
            std::cerr << "xme::Matrix + operator error\n";
        }
    }
    {
        math::Matrix<float, 2, 3> m{math::vec3{5}, math::vec3{-3}};
        auto r1 = m - 2;
        auto r2 = m - math::Matrix<float, 2, 3>{2};

        bool error = r1[0] != math::vec3{3} || r1[1] != math::vec3{-5};
        error |= r2[0] != math::vec3{3, 5, 5} || r2[1] != math::vec3{-3, -5, -3};
        if (error) {
            ++errors;
            std::cerr << "xme::Matrix - operator error\n";
        }
    }
    {
        math::Matrix<float, 2, 3> m{math::vec3{5}, math::vec3{-3}};
        auto r = m * 4;

        bool error = r[0] != math::vec3{20} || r[1] != math::vec3{-12};
        if (error) {
            ++errors;
            std::cerr << "xme::Matrix * operator error\n";
        }
    }
    {
        math::Matrix<float, 2, 3> m{math::vec3{16}, math::vec3{120}};
        auto r = m / 4;

        bool error = r[0] != math::vec3{4} || r[1] != math::vec3{30};
        if (error) {
            ++errors;
            std::cerr << "xme::Matrix / operator error\n";
        }
    }
    return errors;
}

int test_basic_functions() {
    int errors = 0;
    {
        math::mat2 m{math::vec2{3, 4}, math::vec2{1, 2}};
        bool error = m.determinant() != 2;

        if (error) {
            std::cerr << "xme::Matrix::determinant 2 error\n";
            ++errors;
        }
    }
    {
        math::mat3 m{math::vec3{5, 3, 1}, math::vec3{7, 3, 4}, math::vec3{9, 7, 8}};
        bool error = m.determinant() != -58;
        if (error) {
            std::cerr << "xme::Matrix::determinant 3 error\n";
            ++errors;
        }
    }
    {
        math::mat4 m{math::vec4{5, 3, 1, 2}, math::vec4{6, 7, 3, 4},
                     math::vec4{9, 7, -3, 8}, math::vec4{9, -8, -3, 1}};
        bool error = m.determinant() != -498;
        if (error) {
            std::cerr << "xme::Matrix::determinant 4 error\n";
            ++errors;
        }
    }
    return errors;
}

int main() {
    int errors = 0;
    errors += test_access();
    errors += test_operators();
    errors += test_basic_functions();
    return errors;
}
