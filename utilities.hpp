#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <ostream>
#include <tuple>
#include <functional>

#if (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L || __cplusplus >= 202002L) //C++20
    #include <aggregate_size.hpp>
#else
    #include <utility>
#endif //C++20

namespace tutils {

    template <typename Tuple>
    using make_tuple_index_seq = std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>;

    template <std::size_t I, typename Tuple>
    using tuple_element_t = std::tuple_element_t<I, std::remove_reference_t<Tuple>>;

    template <std::size_t I, typename Tuple>
    using lvalue_ref_tuple_element_t = std::add_lvalue_reference_t<tuple_element_t<I, Tuple>>;

    template <template <typename...> typename Trait, typename Fn, typename Tuple, typename Indices>
    struct trait_for_each_impl {};

    template <template <typename...> typename Trait, typename Fn, typename Tuple, std::size_t... Is>
    struct trait_for_each_impl<Trait, Fn, Tuple, std::index_sequence<Is...>> :
            std::bool_constant<(... && (Trait<Fn, lvalue_ref_tuple_element_t<Is, Tuple>>::value ||
                                        Trait<Fn, tuple_element_t<Is, Tuple>>::value))> {};

    template <template <typename...> typename Trait, typename Fn, typename Tuple>
    struct trait_for_each : trait_for_each_impl<Trait, Fn, Tuple, make_tuple_index_seq<Tuple>> {};

    namespace detail {

        template <typename Tuple, typename Fn, std::size_t... Is>
        constexpr void for_each(Tuple&& tuple, Fn&& fn, std::index_sequence<Is...>)
        noexcept(trait_for_each<std::is_nothrow_invocable, Fn, Tuple>::value) {
            (std::invoke(std::forward<Fn>(fn), std::get<Is>(std::forward<Tuple>(tuple))), ...);
        }

        template <typename T, typename Tuple, std::size_t... Is>
        [[nodiscard]] constexpr T from_tuple(Tuple&& tuple, std::index_sequence<Is...>) {
            return {std::get<Is>(std::forward<Tuple>(tuple))...};
        }

        template <typename CharT, typename Traits, typename Arg>
        void put_arg_to_ostream(std::basic_ostream<CharT, Traits>& os, const Arg& arg, std::size_t& args_count) {
            if constexpr (std::is_constructible_v<std::basic_string_view<CharT, Traits>, decltype(arg)>) {
                os << '\"' << std::basic_string_view<CharT, Traits>(arg) << '\"';
            }
            else {
                os << arg;
            }

            if (--args_count >= 1) {
                os << ", ";
            }
        }

    } //namespace detail

    /**
     * @brief calls callable object <b>fn</b> for each element of tuple-like object <b>tuple</b>
    */
    template <typename Tuple, typename Fn>
    constexpr void for_each(Tuple&& tuple, Fn&& fn)
    noexcept(trait_for_each<std::is_nothrow_invocable, Fn, Tuple>::value) {
        detail::for_each(std::forward<Tuple>(tuple), std::forward<Fn>(fn), make_tuple_index_seq<Tuple>{});
    }

    /**
     * @brief returns a type <b>T</b> constructed from a tuple-like object <b>tuple</b>
     * @tparam T the type to construct
    */
    template <typename T, typename Tuple>
    [[nodiscard]] constexpr T from_tuple(Tuple&& tuple) {
        return detail::from_tuple<T>(std::forward<Tuple>(tuple), make_tuple_index_seq<Tuple>{});
    }

    /**
     * @brief passes a tuple-like object <b>tuple</b> to the <b>ostream</b>
     * @return ostream
    */
    template <typename CharT, typename Traits, typename Tuple>
    std::basic_ostream<CharT, Traits>& operator << (std::basic_ostream<CharT, Traits>& os, const Tuple& tuple) {
        std::size_t args_count = std::tuple_size_v<Tuple>;
        os << '{';
        for_each(tuple, [&](const auto& arg) {detail::put_arg_to_ostream(os, arg, args_count);});
        os << '}';

        return os;
    }

    /**
     * @brief pass values from <b>istream</b> to a tuple-like object <b>tuple</b>
     * @return istream
    */
    template <typename CharT, typename Traits, typename Tuple>
    std::basic_istream<CharT, Traits>& operator >> (std::basic_istream<CharT, Traits>& is, Tuple& tuple) {
        for_each(tuple, [&](auto& arg){is >> arg;});
        return is;
    }

} //namespace tutils

#endif //UTILITIES_HPP
