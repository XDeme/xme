#pragma once
#include <functional>
#include <xme/container/tuple.hpp>
#include <xme/core/utility/forward_like.hpp>
#include <xme/core/concepts/different_from.hpp>

namespace xme {
namespace detail {
template<typename F, typename... Args>
struct BindFront {
private:
    using bound_indices = std::make_index_sequence<sizeof...(Args)>;

public:
    constexpr BindFront(const BindFront&) noexcept = default;

    constexpr BindFront(BindFront&&) noexcept = default;

    template<CDifferentFromSelf<BindFront> Fn, typename... BoundArgs>
    explicit constexpr BindFront(Fn&& func, BoundArgs&&... args)
      noexcept(std::is_nothrow_constructible_v<F, Fn>
               && (std::is_nothrow_constructible_v<BoundArgs, Args> && ...)) :
      m_callable(std::forward<Fn>(func)), m_bound_args{std::forward<BoundArgs>(args)...} {}

    constexpr auto operator=(const BindFront&) noexcept -> BindFront& = default;

    constexpr auto operator=(BindFront&&) noexcept -> BindFront& = default;

    // TODO: Simplify when deducing this is added
    template<typename... CallArgs>
    constexpr auto operator()(CallArgs&&... args) & noexcept(
      std::is_nothrow_invocable_v<F&, Args&..., CallArgs...>) -> decltype(auto) {
        return BindFront::call(*this, bound_indices{}, std::forward<CallArgs>(args)...);
    }

    template<typename... CallArgs>
    constexpr auto operator()(CallArgs&&... args) const& noexcept(
      std::is_nothrow_invocable_v<const F&, const Args&..., CallArgs...>) -> decltype(auto) {
        return BindFront::call(*this, bound_indices{}, std::forward<CallArgs>(args)...);
    }

    template<typename... CallArgs>
    constexpr auto operator()(CallArgs&&... args) && noexcept(
      std::is_nothrow_invocable_v<F, Args..., CallArgs...>) -> decltype(auto) {
        return BindFront::call(std::move(*this), bound_indices{}, std::forward<CallArgs>(args)...);
    }

    template<typename... CallArgs>
    constexpr auto operator()(CallArgs&&... args) const&& noexcept(
      std::is_nothrow_invocable_v<const F, const Args..., CallArgs...>) -> decltype(auto) {
        return BindFront::call(std::move(*this), bound_indices{}, std::forward<CallArgs>(args)...);
    }

private:
    template<typename T, std::size_t... I, typename... InArgs>
    static constexpr auto call(T&& self, std::index_sequence<I...>, InArgs&&... args)
      noexcept(noexcept(std::invoke(std::forward<T>(self).m_callable,
                                    get<I>(std::forward<T>(self).m_bound_args)...,
                                    std::forward<InArgs>(args)...))) -> decltype(auto) {
        return std::invoke(std::forward<T>(self).m_callable,
                           get<I>(std::forward<T>(self).m_bound_args)...,
                           std::forward<InArgs>(args)...);
    }

    [[no_unique_address]]
    F m_callable;
    [[no_unique_address]]
    xme::Tuple<Args...> m_bound_args;
};

template<typename F, typename... Args>
struct BindBack {
private:
    using bound_indices = std::make_index_sequence<sizeof...(Args)>;

public:
    constexpr BindBack(const BindBack&) noexcept = default;

    constexpr BindBack(BindBack&&) noexcept = default;

    template<CDifferentFromSelf<BindBack> Fn, typename... BoundArgs>
    explicit constexpr BindBack(Fn&& func, BoundArgs&&... args)
      noexcept(std::is_nothrow_constructible_v<F, Fn>
               && (std::is_nothrow_constructible_v<Args, BoundArgs> && ...)) :
      m_callable(std::forward<Fn>(func)), m_bound_args{std::forward<BoundArgs>(args)...} {}

    constexpr auto operator=(const BindBack&) noexcept -> BindBack& = default;

    constexpr auto operator=(BindBack&&) noexcept -> BindBack& = default;

    // TODO: Simplify when deducing this is added
    template<typename... CallArgs>
    constexpr auto operator()(CallArgs&&... args) & noexcept(
      std::is_nothrow_invocable_v<F&, CallArgs..., Args&...>) -> decltype(auto) {
        return BindBack::call(*this, bound_indices{}, std::forward<CallArgs>(args)...);
    }

    template<typename... CallArgs>
    constexpr auto operator()(CallArgs&&... args) const& noexcept(
      std::is_nothrow_invocable_v<const F&, CallArgs..., const Args&...>) -> decltype(auto) {
        return BindBack::call(*this, bound_indices{}, std::forward<CallArgs>(args)...);
    }

    template<typename... CallArgs>
    constexpr auto operator()(CallArgs&&... args) && noexcept(
      std::is_nothrow_invocable_v<F, CallArgs..., Args...>) -> decltype(auto) {
        return BindBack::call(std::move(*this), bound_indices{}, std::forward<CallArgs>(args)...);
    }

    template<typename... CallArgs>
    constexpr auto operator()(CallArgs&&... args) const&& noexcept(
      std::is_nothrow_invocable_v<const F, CallArgs..., const Args...>) -> decltype(auto) {
        return BindBack::call(std::move(*this), bound_indices{}, std::forward<CallArgs>(args)...);
    }

private:
    template<typename T, std::size_t... I, typename... InArgs>
    static constexpr auto call(T&& self, std::index_sequence<I...>, InArgs&&... args)
      noexcept(noexcept(std::invoke(std::forward<T>(self).m_callable, std::forward<InArgs>(args)...,
                                    get<I>(std::forward<T>(self).m_bound_args)...)))
        -> decltype(auto) {
        return std::invoke(std::forward<T>(self).m_callable,
                           std::forward<InArgs>(args)...,
                           get<I>(std::forward<T>(self).m_bound_args)...);
    }

    [[no_unique_address]]
    F m_callable;
    [[no_unique_address]]
    xme::Tuple<Args...> m_bound_args;
};
}  // namespace detail

template<typename F, typename... Args>
constexpr auto bind_front(F&& func, Args&&... args) {
    static_assert(std::is_constructible_v<std::decay_t<F>, F>);
    static_assert(std::is_move_constructible_v<std::decay<F>>);
    static_assert((std::is_constructible_v<std::decay_t<Args>, Args> && ...));
    static_assert((std::is_move_constructible_v<std::decay_t<Args>> && ...));

    return detail::BindFront<std::decay_t<F>, std::decay_t<Args>...>(std::forward<F>(func),
                                                                     std::forward<Args>(args)...);
}
#if defined(__cpp_explicit_this_parameter)
template<auto f, typename... Args>
constexpr auto bindFront(Args&&... args) {
    static_assert((std::is_constructible_v<std::decay_t<Args>, Args> && ...));
    static_assert((std::is_move_constructible_v<std::decay<Args>> && ...));

    return [... bound_args(std::forward<Args>(args))]<typename Self, typename... T>(
             this Self&&, T&&... call_args) -> decltype(auto) {
        return std::invoke(f, xme::forwardLike<Self>(bound_args)..., std::forward<T>(call_args)...);
    };
}
#endif

template<typename F, typename... Args>
constexpr auto bind_back(F&& func, Args&&... args) {
    static_assert(std::is_constructible_v<std::decay_t<F>, F>);
    static_assert(std::is_move_constructible_v<std::decay<F>>);
    static_assert((std::is_constructible_v<std::decay_t<Args>, Args> && ...));
    static_assert((std::is_move_constructible_v<std::decay_t<Args>> && ...));

    return detail::BindBack<std::decay_t<F>, std::decay_t<Args>...>(std::forward<F>(func),
                                                                    std::forward<Args>(args)...);
}

#if defined(__cpp_explicit_this_parameter)
template<auto f, typename... Args>
constexpr auto bindBack(Args&&... args) {
    static_assert((std::is_constructible_v<std::decay_t<Args>, Args> && ...));
    static_assert((std::is_move_constructible_v<std::decay<Args>> && ...));

    return [... bound_args(std::forward<Args>(args))]<typename Self, typename... T>(
             this Self&&, T&&... call_args) -> decltype(auto) {
        return std::invoke(f, xme::forwardLike<Self>(bound_args)..., std::forward<T>(call_args)...);
    };
}
#endif
}  // namespace xme
