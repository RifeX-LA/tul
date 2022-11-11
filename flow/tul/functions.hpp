#pragma once

#include <functional>
#include <flow/tul/traits.hpp>

namespace flow::tul {

    namespace detail {

        template <typename Tuple, typename Fn, std::size_t... Is>
        constexpr void for_each(Tuple&& tuple, Fn&& fn, std::index_sequence<Is...>)
        noexcept(detail::trait_for_each<std::is_nothrow_invocable, Fn, Tuple>::value) {
            (std::invoke(std::forward<Fn>(fn), std::get<Is>(std::forward<Tuple>(tuple))), ...);
        }

        template <typename T, typename Tuple, std::size_t... Is>
        [[nodiscard]] constexpr T from_tuple(Tuple&& tuple, std::index_sequence<Is...>) {
            return {std::get<Is>(std::forward<Tuple>(tuple))...};
        }

    } //namespace detail

    /**
     * @brief calls callable object <b>fn</b> for each element of tuple-like object <b>tuple</b>
    */
    template <typename Tuple, typename Fn>
    constexpr void for_each(Tuple&& tuple, Fn&& fn)
    noexcept(detail::trait_for_each<std::is_nothrow_invocable, Fn, Tuple>::value) {
        detail::for_each(std::forward<Tuple>(tuple), std::forward<Fn>(fn), detail::make_tuple_index_seq<Tuple>{});
    }

    /**
     * @brief returns a type <b>T</b> constructed from a tuple-like object <b>tuple</b>
     * @tparam T the type to construct
    */
    template <typename T, typename Tuple>
    [[nodiscard]] constexpr T from_tuple(Tuple&& tuple) {
        return detail::from_tuple<T>(std::forward<Tuple>(tuple), detail::make_tuple_index_seq<Tuple>{});
    }

} //namespace flow::tul
