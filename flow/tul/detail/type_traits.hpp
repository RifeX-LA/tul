#pragma once

#include <tuple>

namespace flow::tul::detail {

    template <typename Tuple>
    struct tuple_size : public std::tuple_size<std::remove_reference_t<Tuple>> {};

    template <typename Tuple>
    inline constexpr std::size_t tuple_size_v = tuple_size<Tuple>::value;


    //
    // tuple helping aliases
    //

    template <std::size_t I, typename Tuple>
    struct tuple_element : std::tuple_element<I, std::remove_reference_t<Tuple>> {};

    template <std::size_t I, typename Tuple>
    using tuple_element_t = typename tuple_element<I, Tuple>::type;


    template <std::size_t I, typename Tuple>
    struct l_ref_tuple_element : std::add_lvalue_reference<tuple_element_t<I, Tuple>> {};

    template <std::size_t I, typename Tuple>
    using l_ref_tuple_element_t = typename l_ref_tuple_element<I, Tuple>::type;


    template <typename Tuple>
    using make_tuple_index_seq = std::make_index_sequence<tuple_size_v<Tuple>>;


    //
    // tuple_all_of
    //

    template <template <typename...> typename Trait, typename Fn, typename Tuple, typename Indices>
    struct tuple_all_of_impl {};

    template <template <typename...> typename Trait, typename Fn, typename Tuple, std::size_t... Is>
    struct tuple_all_of_impl<Trait, Fn, Tuple, std::index_sequence<Is...>> :
            std::bool_constant<(... && (Trait<Fn, l_ref_tuple_element_t<Is, Tuple>>::value ||
                                        Trait<Fn, tuple_element_t<Is, Tuple>>::value))> {
    };

    template <template <typename...> typename Trait, typename Fn, typename Tuple>
    struct tuple_all_of : tuple_all_of_impl<Trait, Fn, Tuple, make_tuple_index_seq<Tuple>> {};

    template <template <typename...> typename Trait, typename Fn, typename Tuple>
    inline constexpr bool tuple_all_of_v = tuple_all_of<Trait, Fn, Tuple>::value;


    //
    // any_of
    //

    template<template<typename...> typename Trait, typename T, typename... Args>
    struct any_of : std::bool_constant<(... || Trait<T, Args>::value)> {};

    template<template<typename...> typename Trait, typename T, typename... Args>
    inline constexpr bool any_of_v = any_of<Trait, T, Args...>::value;


    //
    // is_char
    //

#ifdef __cpp_char8_t
    template <typename T>
    struct is_char : any_of<std::is_same, T, char, signed char, unsigned char, wchar_t, char8_t, char16_t, char32_t> {};
#else
    template <typename T>
    struct is_char : trait_is_any<std::is_same, T, char, signed char, unsigned char, wchar_t, char16_t, char32_t> {};
#endif

    template <typename T>
    inline constexpr bool is_char_v = is_char<T>::value;

} // namespace flow::tul::detail
