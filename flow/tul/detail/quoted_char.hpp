#pragma once

#include <iostream>

namespace flow::tul::detail {

    template <typename CharT, typename Delim>
    struct quoted_char {
        CharT symbol;
        Delim delim;

        constexpr quoted_char(CharT c, Delim delim) noexcept : symbol(std::forward<CharT>(c)), delim(delim) {}

        quoted_char(const quoted_char&) = delete;
    };

    template <typename CharT, typename Traits, typename Delim>
    std::basic_ostream<Delim, Traits>& operator<<(std::basic_ostream<Delim, Traits>& os, const quoted_char<CharT, Delim>& view) {
        os << view.delim << view.symbol << view.delim;
        return os;
    }

    template <typename CharT, typename Traits, typename Delim>
    std::basic_istream<Delim, Traits>& operator>>(std::basic_istream<Delim, Traits>& is, const quoted_char<CharT, Delim>& view) {
        Delim delim;
        is >> delim >> view.symbol >> delim;
        return is;
    }

} // namespace flow::tul::detail
