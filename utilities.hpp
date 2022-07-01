#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <utility>
#include <ostream>

namespace cpp {

#if __cplusplus >= 202002L

    // Get fields count of an aggregate type

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
        struct aggregate_init_with_n_args_impl<T, std::index_sequence<Is...>> : std::bool_constant<aggregate_initializable<T, indexed_any<Is>...>> {};

        template <typename T, std::size_t N>
        concept aggregate_init_with_n_args = aggregate_init_with_n_args_impl<T, std::make_index_sequence<N>>::value;

        template <typename T, typename Is, typename NestedIs>
        struct nested_aggregate_init_impl {};

        template <typename T, std::size_t ... Is, std::size_t ... NestedIs>
        struct nested_aggregate_init_impl<T, std::index_sequence<Is...>, std::index_sequence<NestedIs...>> :
                std::bool_constant<requires { T{std::declval<indexed_any<Is>>()..., {std::declval<indexed_any<NestedIs>>()...}}; }> {};

        template <typename T, std::size_t N, std::size_t M>
        concept nested_aggregate_init = nested_aggregate_init_impl<T, std::make_index_sequence<N>, std::make_index_sequence<M>>::value;

        template <typename T, std::size_t N>
        concept initializable = (N <= sizeof(T));

        template <typename T, std::size_t N, std::size_t M, bool IsInitializable>
        struct nested_aggregate_size_impl : nested_aggregate_size_impl<T, N, M + 1, initializable<T, M + 1> && nested_aggregate_init<T, N, M + 1>> {};

        template <typename T, std::size_t N, std::size_t M>
        struct nested_aggregate_size_impl<T, N, M, false> : std::integral_constant<std::size_t, M - 1> {};

        template <typename T, std::size_t N>
        struct nested_aggregate_size : nested_aggregate_size_impl<T, N, 0, true> {};

        template <typename T, std::size_t N, bool IsInitializable>
        struct aggregate_with_nested_size_impl : aggregate_with_nested_size_impl<T, N + 1, aggregate_init_with_n_args<T, N + 1>> {};

        template <typename T, std::size_t N>
        struct aggregate_with_nested_size_impl<T, N, false> : std::integral_constant<std::size_t, N - 1> {};

        template <typename T>
        struct aggregate_with_nested_size : aggregate_with_nested_size_impl<T, 0, true> {};

        template <std::size_t NestedSize, std::size_t TotalFields>
        constexpr std::size_t nested_step = NestedSize > TotalFields ? 1 : NestedSize;

        template <typename T, std::size_t CurField, std::size_t TotalFields, std::size_t UniqueFields>
        struct aggregate_size_impl : aggregate_size_impl<T, CurField + nested_step<nested_aggregate_size<T, CurField>::value, TotalFields>, TotalFields, UniqueFields + 1> {};

        template <typename T, std::size_t TotalFields, std::size_t UniqueFields>
        struct aggregate_size_impl<T, TotalFields, TotalFields, UniqueFields> : std::integral_constant<std::size_t, UniqueFields> {};
    }

    template <typename T>
    struct aggregate_size : detail::aggregate_size_impl<T, 0, detail::aggregate_with_nested_size<T>::value, 0> {};

    template <typename T>
    constexpr std::size_t aggregate_size_v = aggregate_size<T>::value;

#endif

    template <typename Tuple>
    using make_tuple_index_seq = std::make_index_sequence<std::tuple_size_v<Tuple>>;

    // From tuple-like objects to struct conversion

    template <typename T, typename Tuple, std::size_t ... Is>
    [[nodiscard]] constexpr T _from_tuple(Tuple&& tuple, std::index_sequence<Is...>) {
        return {std::get<Is>(std::forward<Tuple>(tuple))...};
    }

    template <typename T, typename Tuple>
    [[nodiscard]] constexpr T from_tuple(Tuple&& tuple) {
        return cpp::_from_tuple<T>(std::forward<Tuple>(tuple), make_tuple_index_seq<std::decay_t<Tuple>>{});
    }

    // Passing tuple-like objects to ostream

    template <std::size_t I, typename CharT, typename Traits, typename Tuple>
    void _put_value_to_ostream(std::basic_ostream<CharT, Traits>& os, const Tuple& tuple, std::size_t tuple_size) {
        os << std::get<I>(tuple);
        if (I < tuple_size - 1) {
            os << ", ";
        }
    }

    template <typename CharT, typename Traits, typename Tuple, std::size_t ... Is>
    void _put_to_ostream(std::basic_ostream<CharT, Traits>& os, const Tuple& tuple, std::index_sequence<Is...>) {
        os << '{';
        (cpp::_put_value_to_ostream<Is>(os, tuple, sizeof...(Is)), ...);
        os << '}';
    }

    template <typename CharT, typename Traits, typename Tuple>
    std::basic_ostream<CharT, Traits>& operator << (std::basic_ostream<CharT, Traits>& os, const Tuple& tuple) {
        cpp::_put_to_ostream(os, tuple, make_tuple_index_seq<Tuple>{});
        return os;
    }

    // Passing values from istream to tuple-like object

    template <typename CharT, typename Traits, typename Tuple, std::size_t ... Is>
    void _put_from_istream(std::basic_istream<CharT, Traits>& is, Tuple& tuple, std::index_sequence<Is...>) {
       (is >> ... >> std::get<Is>(tuple));
    }

    template <typename CharT, typename Traits, typename Tuple>
    std::basic_istream<CharT, Traits>& operator >> (std::basic_istream<CharT, Traits>& is, Tuple& tuple) {
        cpp::_put_from_istream(is, tuple, make_tuple_index_seq<Tuple>{});
        return is;
    }

}

#endif //UTILITIES_HPP
