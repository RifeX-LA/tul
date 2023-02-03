#pragma once

#include <flow/tul/detail/aggregate_size.hpp>

namespace flow::tul {

    template <typename T>
    struct aggregate_size : detail::aggregate_size_impl<T, 0, detail::full_aggregate_size<T>::value, 0> {};

    template <typename T>
    inline constexpr std::size_t aggregate_size_v = aggregate_size<T>::value;

} // namespace flow::tul
