#include <iostream>
#include <xme/core/iterators/contiguous_iterator.hpp>
#include <xme/core/iterators/reverse_iterator.hpp>

template<typename T>
using iterator = xme::ReverseIterator<T>;

static_assert(std::random_access_iterator<iterator<float*>>);

int test_increment() {
    int errors = 0;
    int arr[3]{4, 1, -2};
    iterator<int*> rbegin{arr + 3};
    iterator<const int*> rend{arr};
    {
        bool error = (rbegin + 3) != rend || (3 + rbegin) != rend;
        iterator<int*> it1{rbegin};
        error |= it1++ != rbegin;
        error |= ++it1 != rbegin + 2;
        if(error) {
            std::cerr << "xme::ReverseIterator increment 1 error";
            ++errors;
        }
    }
    {
        iterator<const int*> it1{rbegin};
        it1 += 2;
        bool error = it1 != rbegin + 2;
        it1 += -2;
        error |= it1 != rbegin;
        if(error) {
            std::cerr << "xme::ReverseIterator increment 2 error\n";
            ++errors;
        }
    }
    return errors;
}

int test_decrement() {
    int errors = 0;
    int arr[3]{4, 1, -2};
    iterator<int*> rbegin{arr + 3};
    iterator<const int*> rend{arr};
    {
        bool error = (rend - 3) != rbegin || (rend - rbegin) != 3;
        iterator<const int*> it1{rend};
        error |= it1-- != rend;
        error |= --it1 != rend - 2;
        if(error) {
            std::cerr << "xme::ReverseIterator decrement 1 error";
            ++errors;
        }
    }
    {
        iterator<const int*> it1{rend};
        it1 -= 2;
        bool error = it1 != rbegin + 1;
        it1 -= -2;
        error |= it1 != rend;
        if(error) {
            std::cerr << "xme::ReverseIterator decrement 2 error\n";
            ++errors;
        }
    }
    return errors;
}

int test_access() {
    int errors = 0;
    int arr[3]{4, 1, -2};
    iterator<int*> rbegin{arr + 3};
    iterator<const int*> rend{arr};
    {
        bool error = rbegin[0] != -2 || rbegin[1] != 1 || rbegin[2] != 4;
        error |= rend[-1] != 4 || rend[-2] != 1 || rend[-3] != -2;
        error |= *rbegin != -2 || *(rbegin + 1) != 1 || *(rbegin + 2) != 4;
        error |= *((rbegin).operator->()) != -2 || *((rbegin + 1).operator->()) != 1;
        error |= *((rbegin + 2).operator->()) != 4;
        if(error) {
            std::cerr << "xme::ReverseIterator access error\n";
            ++errors;
        }
    }
    return errors;
}

int main() {
    int errors = 0;
    errors += test_increment();
    errors += test_decrement();
    errors += test_access();
    return errors;
}
