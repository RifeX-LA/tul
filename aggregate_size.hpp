#ifndef AGGREGATE_SIZE_HPP
#define AGGREGATE_SIZE_HPP

#include <utility>

namespace tutils {

    namespace detail {

        constexpr std::size_t mid(std::size_t left, std::size_t right) noexcept {
            return left + (right - left) / 2;
        }

        template <std::size_t>
        struct indexed_any {
            template <typename T>
            constexpr operator T() const noexcept;
        };

        template <template <std::size_t> typename Predicate, std::size_t Begin, std::size_t End>
        struct binary_search : std::conditional_t<End - Begin <= 1,
                std::integral_constant<std::size_t, Begin>,
                std::conditional_t<Predicate<mid(Begin, End)>::value,
                    binary_search<Predicate, mid(Begin, End), End>,
                    binary_search<Predicate, Begin, mid(Begin, End)>>> {};


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


        template <typename T>
        struct aggregate_wrapper {
            template <std::size_t N>
            struct init_with_n_args : std::bool_constant<aggregate_init_with_n_args<T, N>> {};
        };

        template <typename T, std::size_t N>
        struct nested_aggregate_wrapper {
            template <std::size_t M>
            struct init_with_n_args : std::bool_constant<nested_aggregate_init<T, N, M>> {};
        };

        template <typename T>
        struct full_aggregate_size : binary_search<aggregate_wrapper<T>::template init_with_n_args, 0, 8 * sizeof(T) + 1> {};

        template <typename T, std::size_t N>
        struct nested_aggregate_size : binary_search<nested_aggregate_wrapper<T, N>::template init_with_n_args, 0, 8 * sizeof(T) + 1> {};

        template <std::size_t NestedSize, std::size_t TotalFields>
        constexpr std::size_t offset = NestedSize > TotalFields ? 1 : NestedSize;

        template <typename T, std::size_t CurrField, std::size_t TotalFields, std::size_t Size>
        struct aggregate_size_impl : aggregate_size_impl<T,
                CurrField + offset<nested_aggregate_size<T, CurrField>::value, TotalFields>, TotalFields, Size + 1> {};

        template <typename T, std::size_t TotalFields, std::size_t Size>
        struct aggregate_size_impl<T, TotalFields, TotalFields, Size> : std::integral_constant<std::size_t, Size> {};

    } //namespace detail

    template <typename T>
    struct aggregate_size : detail::aggregate_size_impl<T, 0, detail::full_aggregate_size<T>::value, 0> {};

    template <typename T>
    constexpr std::size_t aggregate_size_v = aggregate_size<T>::value;

} //namespace tutils

#endif //AGGREGATE_SIZE_HPP
