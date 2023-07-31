#include <xme/math/math.hpp>
#include <algorithm>
#include <iostream>

bool isError(bool b) { return b == false; }

int testOperators() {
    int errors = 0;
#define TEST(op) {\
        constexpr xme::dvec2 v{5}; \
        constexpr auto r1 = v op 5; \
        constexpr auto r2 = 15 op v; \
        constexpr auto r3 = xme::dvec2{0, 5} op v; \
        \
        std::array<bool, 6> results{}; \
        for(std::int32_t i = 0; i < 2; ++i) { \
            results[i] = r1[i] == (5 op 5); \
            results[2+i] = r2[i] == (15 op 5); \
            results[4+i] = r3[i] == ((5*i) op 5); \
        } \
        \
        auto error = std::ranges::any_of(results, isError); \
        if(error) { \
            std::cerr << "Vector " #op " Operator Error\n"; \
            ++errors; \
        } \
    }
    
    TEST(+)
    TEST(-)
    TEST(*)
    TEST(/)
    return errors;
}

int main() {
    int errors = 0;
    errors += testOperators();
    return errors;
}