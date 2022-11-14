#pragma once

#include <tuple>

namespace flow::tul::detail {

    template <typename Tuple>
    inline constexpr std::size_t tuple_size_v = std::tuple_size_v<std::remove_reference_t<Tuple>>;

    ///////////////// HELPING ALIASES

    template<typename Tuple>
    using make_tuple_index_seq = std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>;

    template<std::size_t I, typename Tuple>
    using tuple_element_t = std::tuple_element_t<I, std::remove_reference_t<Tuple>>;

    template<std::size_t I, typename Tuple>
    using l_ref_tuple_element_t = std::add_lvalue_reference_t<tuple_element_t<I, Tuple>>;

    ///////////////// TRAIT_FOR_EACH

    template<template<typename...> typename Trait, typename Fn, typename Tuple, typename Indices>
    struct trait_for_each_impl {};

    template<template<typename...> typename Trait, typename Fn, typename Tuple, std::size_t... Is>
    struct trait_for_each_impl<Trait, Fn, Tuple, std::index_sequence<Is...>> :
            std::bool_constant<(... && (Trait<Fn, l_ref_tuple_element_t<Is, Tuple>>::value ||
                                        Trait<Fn, tuple_element_t<Is, Tuple>>::value))> {
    };

    template<template<typename...> typename Trait, typename Fn, typename Tuple>
    struct trait_for_each : trait_for_each_impl<Trait, Fn, Tuple, make_tuple_index_seq<Tuple>> {};

} //namespace flow::tul::detail