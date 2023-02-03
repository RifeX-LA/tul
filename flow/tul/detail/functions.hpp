#pragma once

#include <functional>
#include <flow/tul/detail/type_traits.hpp>

namespace flow::tul::detail {

    template <typename Tuple, typename Fn, std::size_t... Is>
    constexpr void for_each(Tuple&& tuple, Fn&& fn, std::index_sequence<Is...>)
    noexcept(detail::tuple_all_of_v<std::is_nothrow_invocable, Fn, Tuple>) {
        (std::invoke(std::forward<Fn>(fn), std::get<Is>(std::forward<Tuple>(tuple))), ...);
    }

    template <typename T, typename Tuple, std::size_t... Is>
    [[nodiscard]] constexpr T from_tuple(Tuple&& tuple, std::index_sequence<Is...>) {
        return T{std::get<Is>(std::forward<Tuple>(tuple))...};
    }

} // namespace flow::tul::detail
