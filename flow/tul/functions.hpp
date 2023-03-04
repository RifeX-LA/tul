#pragma once

#include <flow/tul/detail/functions.hpp>

namespace flow::tul {

    /**
     * @brief calls callable object <b>fn</b> for each element of tuple-like object <b>tuple</b>
    */
    template <typename Tuple, typename Fn>
    constexpr void for_each(Tuple&& tuple, Fn&& fn)
    noexcept(detail::tuple_all_of_v<std::is_nothrow_invocable, Fn, Tuple>) {
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

} // namespace flow::tul
