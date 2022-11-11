#pragma once

#include <ostream>
#include <iomanip>
#include <flow/tul/functions.hpp>

namespace flow::tul {

    namespace detail {

        template <typename Tuple>
        struct io_impl {
            Tuple&& tuple;

            template <typename CharT, typename Traits, typename Arg>
            static void pass_value_to_ostream(std::basic_ostream<CharT, Traits>& os, Arg&& arg, std::size_t& args_count) {
                if constexpr (std::is_constructible_v<std::basic_string_view<CharT, Traits>, Arg>) {
                    os << std::quoted(std::basic_string_view<CharT, Traits>(std::forward<Arg>(arg)));
                }
                else {
                    os << arg;
                }

                if (--args_count) {
                    os << ", ";
                }
            }

            template <typename CharT, typename Traits>
            friend std::basic_ostream<CharT, Traits>& operator << (std::basic_ostream<CharT, Traits>& os, io_impl&& io) {
                std::size_t args_count = std::tuple_size_v<std::remove_reference_t<Tuple>>;
                auto to_ostream = [&](auto&& arg) {pass_value_to_ostream(os, std::forward<decltype(arg)>(arg), args_count);};

                os << '{';
                flow::tul::for_each(std::forward<Tuple>(io.tuple), to_ostream);
                os << '}';

                return os;
            }

            template <typename CharT, typename Traits>
            friend std::basic_istream<CharT, Traits>& operator >> (std::basic_istream<CharT, Traits>& is, io_impl&& io) {
                flow::tul::for_each(std::forward<Tuple>(io.tuple), [&](auto& arg){is >> arg;});
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