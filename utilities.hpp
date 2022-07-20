#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <ostream>
#include <tuple>
#include <functional>

#if (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L || __cplusplus >= 202002L) //C++20
    #include <aggregate_size.hpp>
#else
    #include <utility>
#endif

namespace tutils {

    template <typename Tuple>
    using make_tuple_index_seq = std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>;

    namespace detail {

        template <typename Tuple, typename Fn, std::size_t ... Is>
        constexpr void for_each(Tuple&& tuple, Fn&& fn, std::index_sequence<Is...>) {
            (std::invoke(std::forward<Fn>(fn), std::get<Is>(std::forward<Tuple>(tuple))), ...);
        }

        template <typename T, typename Tuple, std::size_t ... Is>
        [[nodiscard]] constexpr T from_tuple(Tuple&& tuple, std::index_sequence<Is...>) {
            return {std::get<Is>(std::forward<Tuple>(tuple))...};
        }

        template <std::size_t I, typename CharT, typename Traits, typename Tuple>
        void put_arg_to_ostream(std::basic_ostream<CharT, Traits>& os, const Tuple& tuple, std::size_t tuple_size) {
            const auto& arg = std::get<I>(tuple);
            if constexpr (std::is_constructible_v<std::basic_string_view<CharT, Traits>, decltype(arg)>) {
                os << '\"' << std::basic_string_view<CharT, Traits>(arg) << '\"';
            }
            else {
                os << arg;
            }

            if (I < tuple_size - 1) {
                os << ", ";
            }
        }

        template <typename CharT, typename Traits, typename Tuple, std::size_t ... Is>
        void put_to_ostream(std::basic_ostream<CharT, Traits>& os, const Tuple& tuple, std::index_sequence<Is...>) {
            os << '{';
            (put_arg_to_ostream<Is>(os, tuple, sizeof...(Is)), ...);
            os << '}';
        }

        template <typename CharT, typename Traits, typename Tuple, std::size_t ... Is>
        void put_from_istream(std::basic_istream<CharT, Traits>& is, Tuple& tuple, std::index_sequence<Is...>) {
            (is >> ... >> std::get<Is>(tuple));
        }

    } //namespace detail

    /**
     * @brief calls callable object <b>fn</b> for each element of tuple-like object <b>tuple</b>
    */
    template <typename Tuple, typename Fn>
    constexpr void for_each(Tuple&& tuple, Fn&& fn) {
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
        detail::put_to_ostream(os, tuple, make_tuple_index_seq<Tuple>{});
        return os;
    }

    /**
     * @brief pass values from <b>istream</b> to a tuple-like object <b>tuple</b>
     * @return istream
    */
    template <typename CharT, typename Traits, typename Tuple>
    std::basic_istream<CharT, Traits>& operator >> (std::basic_istream<CharT, Traits>& is, Tuple& tuple) {
        detail::put_from_istream(is, tuple, make_tuple_index_seq<Tuple>{});
        return is;
    }

} //namespace tutils

#endif //UTILITIES_HPP
