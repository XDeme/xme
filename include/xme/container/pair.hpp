#pragma once
#include "concepts.hpp"

namespace xme {
template<typename T, typename U>
struct Pair {
private:
    using self = Pair<T, U>;

public:
    template<typename P>
        requires(CPairLike<std::decay_t<P>>)
    constexpr auto operator=(P&& p) noexcept(std::is_nothrow_swappable_v<T> &&
                                             std::is_nothrow_swappable_v<U>) -> self& {
        first = std::forward<P>(p).first;
        second = std::forward<P>(p).second;
        return *this;
    }

    constexpr auto operator[](std::integral_constant<std::size_t, 0>) & noexcept -> T& {
        return first;
    }

    constexpr auto operator[](std::integral_constant<std::size_t, 0>) const& noexcept
        -> const T& {
        return first;
    }

    constexpr auto operator[](std::integral_constant<std::size_t, 0>) && noexcept -> T&& {
        return static_cast<Pair&&>(*this).first;
    }

    constexpr auto operator[](std::integral_constant<std::size_t, 1>) & noexcept -> U& {
        return second;
    }

    constexpr auto operator[](std::integral_constant<std::size_t, 1>) const& noexcept
        -> const U& {
        return second;
    }

    constexpr auto operator[](std::integral_constant<std::size_t, 1>) && noexcept -> U&& {
        return static_cast<Pair&&>(*this).second;
    }

    constexpr auto operator<=>(const self&) const = default;
    constexpr bool operator==(const self&) const = default;

    constexpr void swap(Pair& p) noexcept(std::is_nothrow_swappable_v<T> &&
                                          std::is_nothrow_swappable_v<U>) {
        std::ranges::swap(first, p.first);
        std::ranges::swap(second, p.second);
    }

    [[no_unique_address]] T first;
    [[no_unique_address]] U second;
};

template<typename T, typename U>
Pair(T, U) -> Pair<std::unwrap_ref_decay_t<T>, std::unwrap_ref_decay_t<U>>;

template<std::size_t I, typename T, typename U>
constexpr auto get(Pair<T, U>& p) noexcept -> decltype(auto) {
    return p[std::integral_constant<std::size_t, I>{}];
}

template<std::size_t I, typename T, typename U>
constexpr auto get(const Pair<T, U>& p) noexcept -> decltype(auto) {
    return p[std::integral_constant<std::size_t, I>{}];
}

template<std::size_t I, typename T, typename U>
constexpr auto get(Pair<T, U>&& p) noexcept -> decltype(auto) {
    return std::move(p)[std::integral_constant<std::size_t, I>{}];
}

template<typename T, typename U>
constexpr void swap(Pair<T, U>& lhs,
                    Pair<T, U>& rhs) noexcept(std::is_nothrow_swappable_v<T> &&
                                              std::is_nothrow_swappable_v<U>) {
    lhs.swap(rhs);
}

template<typename T, typename U>
constexpr auto makePair(T&& t, U&& u) noexcept(
    std::is_nothrow_constructible_v<
        xme::Pair<std::unwrap_ref_decay_t<T>, std::unwrap_ref_decay_t<U>>, T, U>) {
    return Pair<std::unwrap_ref_decay_t<T>, std::unwrap_ref_decay_t<U>>{
        std::forward<T>(t), std::forward<U>(u)};
}

} // namespace xme

namespace std {
template<typename T, typename U>
struct tuple_size<xme::Pair<T, U>> : std::integral_constant<std::size_t, 2> {};

template<typename T, typename U>
struct tuple_element<0, xme::Pair<T, U>> {
    using type = T;
};

template<typename T, typename U>
struct tuple_element<1, xme::Pair<T, U>> {
    using type = U;
};
} // namespace std