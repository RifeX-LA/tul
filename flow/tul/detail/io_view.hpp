#pragma once

#include <sstream>
#include <flow/tul/detail/io_ops.hpp>

namespace flow::tul::detail {

    template <typename Tuple>
    struct io_view {
        Tuple tuple;

        explicit constexpr io_view(Tuple tuple) noexcept : tuple(std::forward<Tuple>(tuple)) {}

        io_view(const io_view&) = delete;
    };

    template <typename CharT, typename Traits, typename Tuple>
    std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const io_view<Tuple>& view) {
        std::basic_ostringstream<CharT, Traits> buffer;
        buffer << CharT('(');
        detail::print(buffer, std::forward<Tuple>(view.tuple), ", ");
        buffer << CharT(')');

        return os << buffer.str();
    }

    template <typename CharT, typename Traits, typename Tuple>
    std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, const io_view<Tuple>& view) {
        detail::validate_next_char(is, CharT('('));
        detail::read(is, std::forward<Tuple>(view.tuple), ", ");
        detail::validate_next_char(is, CharT(')'));

        return is;
    }

    template <typename Tuple, typename CharT = char, typename Traits = std::char_traits<CharT>>
    struct sep_io_view : io_view<Tuple> {
        std::basic_string_view<CharT, Traits> sep;

        constexpr sep_io_view(Tuple tuple, std::basic_string_view<CharT, Traits> sep)
        noexcept : io_view<Tuple>(std::forward<Tuple>(tuple)), sep(sep) {}
    };

    template <typename CharT, typename Traits, typename Tuple>
    std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const sep_io_view<Tuple, CharT, Traits>& view) {
        std::basic_ostringstream<CharT, Traits> buffer;
        detail::print(buffer, std::forward<Tuple>(view.tuple), view.sep);
        return os << buffer.str();
    }

    template <typename CharT, typename Traits, typename Tuple>
    std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, const sep_io_view<Tuple, CharT, Traits>& view) {
        detail::read(is, std::forward<Tuple>(view.tuple), view.sep);
        return is;
    }

} // namespace flow::tul::detail
