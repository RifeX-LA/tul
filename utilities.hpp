#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <utility>
#include <ostream>

namespace cpp {

    // From tuple-like objects to struct conversion

    template <typename StructT, typename Tuple, std::size_t ... Is>
    [[nodiscard]] constexpr StructT _to_struct(Tuple&& tuple, std::index_sequence<Is...>) {
        return {std::get<Is>(std::forward<Tuple>(tuple))...};
    }

    template <typename StructT, typename Tuple>
    [[nodiscard]] constexpr StructT to_struct(Tuple&& tuple) {
        using tuple_t = std::decay_t<Tuple>;

        return cpp::_to_struct<StructT>(std::forward<Tuple>(tuple), std::make_index_sequence<std::tuple_size<tuple_t>{}>{});
    }

    // Passing tuple-like objects to ostream

    template <std::size_t Ind, typename CharT, typename Traits, typename Tuple>
    void _put_value_to_ostream(std::basic_ostream<CharT, Traits>& os, const Tuple& tuple, std::size_t tuple_size) {
        os << std::get<Ind>(tuple);
        if (Ind + 1 < tuple_size) {
            os << ", ";
        }
    }

    template <typename CharT, typename Traits, typename Tuple, std::size_t ... Is>
    void _put_to_ostream(std::basic_ostream<CharT, Traits>& os, const Tuple& tuple, std::index_sequence<Is...> index_seq) {
        os << '{';
        (cpp::_put_value_to_ostream<Is>(os, tuple, index_seq.size()), ...);
        os << '}';
    }

    template <typename CharT, typename Traits, typename Tuple>
    std::basic_ostream<CharT, Traits>& operator << (std::basic_ostream<CharT, Traits>& os, const Tuple& tuple) {
        cpp::_put_to_ostream(os, tuple, std::make_index_sequence<std::tuple_size<Tuple>{}>{});
        return os;
    }

    // Passing values from istream to tuple

    template <typename CharT, typename Traits, typename Tuple, std::size_t ... Is>
    void _put_from_istream(std::basic_istream<CharT, Traits>& is, Tuple& tuple, std::index_sequence<Is...>) {
       (is >> ... >> std::get<Is>(tuple));
    }

    template <typename CharT, typename Traits, typename Tuple>
    std::basic_istream<CharT, Traits>& operator >> (std::basic_istream<CharT, Traits>& is, Tuple& tuple) {
        cpp::_put_from_istream(is, tuple, std::make_index_sequence<std::tuple_size<Tuple>{}>{});
        return is;
    }

}

#endif //UTILITIES_HPP
