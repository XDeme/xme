#include <xme/functional/bind.hpp>
#include "common.hpp"


int main() {
    int errors = 0;
    {
        auto fn1 = xme::bindFront([](int, int&, int&&){});

        static_assert(std::is_invocable_v<decltype(fn1), int, int&, int>);
        static_assert(std::is_invocable_v<decltype(fn1), int, int&, int&&>);
        static_assert(std::is_invocable_v<decltype(fn1), int&, int&, int&&>);
        static_assert(std::is_invocable_v<decltype(fn1), int&&, int&, int&&>);

        auto fn2 = xme::bindFront([](int&&, const int&, const int&&){}, 1);

        static_assert(std::is_invocable_v<decltype(fn2), int&, int&&>);
        static_assert(std::is_invocable_v<decltype(fn2), const int&, const int&&>);
        static_assert(std::is_invocable_v<decltype(fn2), int, int>);
        
        auto fn3 = xme::bindFront([](){});
        static_assert(sizeof(decltype(fn3)) == 1);

        auto fn6 = xme::bindBack([](int, int&, int&&){});

        static_assert(std::is_invocable_v<decltype(fn6), int, int&, int>);
        static_assert(std::is_invocable_v<decltype(fn6), int, int&, int&&>);
        static_assert(std::is_invocable_v<decltype(fn6), int&, int&, int&&>);
        static_assert(std::is_invocable_v<decltype(fn6), int&&, int&, int&&>);

        auto fn7 = xme::bindBack([](const int&, const int&&, int&&){}, 5);
        static_assert(std::is_invocable_v<decltype(fn7), int&, int&&>);
        static_assert(std::is_invocable_v<decltype(fn7), const int&, const int&&>);
        static_assert(std::is_invocable_v<decltype(fn7), int, int>);

        auto fn8 = xme::bindBack([](){});
        static_assert(sizeof(decltype(fn8)) == 1);
    }

    {
        std::vector<short> results;
        auto fn1 = xme::bindFront([](int a, int b){ return xme::Tuple{a, b}; }, 5);
        {
            auto r = fn1(3);
            results.emplace_back(get<0>(r) == 5);
            results.emplace_back(get<1>(r) == 3);
        }

        auto fn2 = xme::bindBack([](int a, int b){ return xme::Tuple{a, b}; }, 5);
        {
            auto r = fn2(3);
            results.emplace_back(get<0>(r) == 3);
            results.emplace_back(get<1>(r) == 5);
        }

        bool error = std::ranges::any_of(results, isError);
        if(error) {
            std::cerr << "bind(Back|Front) variable binding place error\n";
            ++errors;
        }
    }
    return errors;
}