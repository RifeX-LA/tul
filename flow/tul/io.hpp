#pragma once

#include <ostream>
#include <flow/tul/functions.hpp>
#include <flow/tul/detail/io.hpp>

namespace flow::tul {

    namespace detail {

        template <typename Tuple>
        struct io_impl {
            Tuple&& tuple;

            template <typename CharT, typename Traits>
            friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, io_impl&& io) {
                os << '{';
                flow::tul::for_each(std::forward<Tuple>(io.tuple), print_impl<CharT, Traits>{os, tuple_size_v<Tuple>});
                os << '}';

                return os;
            }

            template <typename CharT, typename Traits>
            friend std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, io_impl&& io) {
                validate_next(is, '{');
                flow::tul::for_each(std::forward<Tuple>(io.tuple), read_impl<CharT, Traits>{is, tuple_size_v<Tuple>});
                validate_next(is, '}');

                return is;
            }
        };

    } //namespace detail

    /**
     * @brief returns wrapper, that can be used with iostreams
     * @param tuple tuple-like object to wrap
     */
    template <typename Tuple>
    [[nodiscard]] constexpr auto io(Tuple&& tuple) noexcept {
        return detail::io_impl<Tuple>{std::forward<Tuple>(tuple)};
    }

} //namespace flow::tul