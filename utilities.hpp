#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <utility>
#include <ostream>
#include <tuple>

namespace cpp {

#if __cplusplus >= 202002L

    namespace detail {

        template <std::size_t>
        struct indexed_any {
            template <typename T>
            constexpr operator T() const noexcept;
        };

        template <typename T, typename ... Args>
        concept list_initializable = requires { T{std::declval<Args>()...}; };

        template <typename T, typename ... Args>
        concept aggregate_initializable = std::is_aggregate_v<T> && list_initializable<T, Args...>;

        template <typename T, typename Indices>
        struct aggregate_init_with_n_args_impl {};

        template <typename T, std::size_t ... Is>
        struct aggregate_init_with_n_args_impl<T, std::index_sequence<Is...>> :
                std::bool_constant<aggregate_initializable<T, indexed_any<Is>...>> {};

        template <typename T, std::size_t N>
        concept aggregate_init_with_n_args = aggregate_init_with_n_args_impl<T, std::make_index_sequence<N>>::value;

        template <typename T, typename Is, typename NestedIs>
        struct nested_aggregate_init_impl {};

        template <typename T, std::size_t ... Is, std::size_t ... NestedIs>
        struct nested_aggregate_init_impl<T, std::index_sequence<Is...>, std::index_sequence<NestedIs...>> :
                std::bool_constant<requires { T{std::declval<indexed_any<Is>>()...,
                                                {std::declval<indexed_any<NestedIs>>()...}}; }> {};

        template <typename T, std::size_t N, std::size_t M>
        concept nested_aggregate_init = nested_aggregate_init_impl<T, std::make_index_sequence<N>,
                                                                      std::make_index_sequence<M>>::value;

        template <typename T, std::size_t N>
        concept theoretic_initializable = (N <= sizeof(T));

        template <typename T, std::size_t N, std::size_t M, bool IsInitializable>
        struct nested_aggregate_size_impl : nested_aggregate_size_impl<T, N, M + 1, theoretic_initializable<T, M + 1>
                                                                                    && nested_aggregate_init<T, N, M + 1>> {};

        template <typename T, std::size_t N, std::size_t M>
        struct nested_aggregate_size_impl<T, N, M, false> : std::integral_constant<std::size_t, M - 1> {};

        template <typename T, std::size_t N>
        struct nested_aggregate_size : nested_aggregate_size_impl<T, N, 0, true> {};

        template <typename T, std::size_t N, bool IsInitializable>
        struct aggregate_with_nested_size_impl : aggregate_with_nested_size_impl<T, N + 1,
                                                    aggregate_init_with_n_args<T, N + 1>> {};

        template <typename T, std::size_t N>
        struct aggregate_with_nested_size_impl<T, N, false> : std::integral_constant<std::size_t, N - 1> {};

        template <typename T>
        struct aggregate_with_nested_size : aggregate_with_nested_size_impl<T, 0, true> {};

        template <std::size_t NestedSize, std::size_t TotalFields>
        constexpr std::size_t nested_step = NestedSize > TotalFields ? 1 : NestedSize;

        template <typename T, std::size_t CurrField, std::size_t TotalFields, std::size_t Size>
        struct aggregate_size_impl : aggregate_size_impl<T,
                CurrField + nested_step<nested_aggregate_size<T, CurrField>::value, TotalFields>, TotalFields, Size + 1> {};

        template <typename T, std::size_t TotalFields, std::size_t Size>
        struct aggregate_size_impl<T, TotalFields, TotalFields, Size> : std::integral_constant<std::size_t, Size> {};
    }

    template <typename T>
    struct aggregate_size : detail::aggregate_size_impl<T, 0, detail::aggregate_with_nested_size<T>::value, 0> {};

    template <typename T>
    constexpr std::size_t aggregate_size_v = aggregate_size<T>::value;

#endif // __cplusplus >= 202002L

    template <typename Tuple>
    using make_tuple_index_seq = std::make_index_sequence<std::tuple_size_v<Tuple>>;

    namespace detail {

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

    }

    /**
     * @brief returns a type T constructed from a tuple-like object Tuple
     * @tparam T the type to construct
    */
    template <typename T, typename Tuple>
    [[nodiscard]] constexpr T from_tuple(Tuple&& tuple) {
        return detail::from_tuple<T>(std::forward<Tuple>(tuple), make_tuple_index_seq<std::decay_t<Tuple>>{});
    }

    /**
     * @brief passes a tuple-like object Tuple to the ostream
     * @return ostream
    */
    template <typename CharT, typename Traits, typename Tuple>
    std::basic_ostream<CharT, Traits>& operator << (std::basic_ostream<CharT, Traits>& os, const Tuple& tuple) {
        detail::put_to_ostream(os, tuple, make_tuple_index_seq<Tuple>{});
        return os;
    }

    /**
     * @brief pass values from istream to a tuple-like object Tuple
     * @return istream
    */
    template <typename CharT, typename Traits, typename Tuple>
    std::basic_istream<CharT, Traits>& operator >> (std::basic_istream<CharT, Traits>& is, Tuple& tuple) {
        detail::put_from_istream(is, tuple, make_tuple_index_seq<Tuple>{});
        return is;
    }

}

#endif //UTILITIES_HPP
