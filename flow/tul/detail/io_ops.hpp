#pragma once

#include <iostream>
#include <iterator>
#include <flow/tul/detail/quoted.hpp>

namespace flow::tul::detail {

    template <typename CharT, typename Traits>
    bool validate_next_char(std::basic_istream<CharT, Traits>& is, CharT expected) {
        if (Traits::to_char_type(is.get()) != expected) {
            is.unget();
            is.setstate(std::basic_istream<CharT, Traits>::failbit);
            return false;
        }
        return true;
    }

    template <typename CharT, typename Traits>
    bool validate_next_sep(std::basic_istream<CharT, Traits>& is, std::basic_string_view<CharT, Traits> sep) {
        auto validate = [&](CharT c) {return validate_next_char(is, c);};
        return std::all_of(sep.begin(), sep.end(), validate);
    }

    template <typename CharT, typename Traits>
    bool validate_next_sep(std::basic_istream<CharT, Traits>& is, const char* sep) {
        for (std::size_t i = 0; sep[i] != '\0'; ++i) {
            if (!validate_next_char(is, CharT(sep[i]))) {
                return false;
            }
        }
        return true;
    }

    template <typename CharT, typename Traits, typename Tuple, typename Sep, std::size_t N = 0>
    void print(std::basic_ostream<CharT, Traits>& os, Tuple&& tuple, Sep sep) {
        if constexpr (N < tuple_size_v<Tuple>) {
            os << flow::tul::detail::quoted(std::get<N>(std::forward<Tuple>(tuple)));
            if constexpr (N + 1 < tuple_size_v<Tuple>) {
                os << sep;
            }
            print<CharT, Traits, Tuple, Sep, N + 1>(os, std::forward<Tuple>(tuple), sep);
        }
    }

    template <typename CharT, typename Traits, typename Tuple, typename Sep, std::size_t N = 0>
    bool read(std::basic_istream<CharT, Traits>& is, Tuple&& tuple, Sep sep) {
        if constexpr (N < tuple_size_v<Tuple>) {
            is >> flow::tul::detail::quoted(std::get<N>(std::forward<Tuple>(tuple)));
            if (!is.good()) {
                return false;
            }

            if constexpr (N + 1 < tuple_size_v<Tuple>) {
                if (!validate_next_sep(is, sep)) {
                    return false;
                }
            }
            return read<CharT, Traits, Tuple, Sep, N + 1>(is, std::forward<Tuple>(tuple), sep);
        }
        return true;
    }

} // namespace flow::tul::detail
