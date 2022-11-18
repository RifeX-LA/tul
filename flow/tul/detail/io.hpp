#pragma once

#include <iomanip>

namespace flow::tul::detail {

    template <typename CharT, typename Traits>
    auto formatted(std::basic_string<CharT, Traits>& s) noexcept {
        return std::quoted(s);
    }

    template <typename CharT, typename Traits>
    auto formatted(const std::basic_string<CharT, Traits>& s) noexcept {
        return std::quoted(s);
    }

    template <typename CharT>
    auto formatted(const CharT* s) noexcept {
        return std::quoted(s);
    }

    template <typename CharT, typename Traits>
    auto formatted(std::basic_string_view<CharT, Traits> sv) noexcept {
        return std::quoted(sv);
    }

    template <typename T>
    decltype(auto) formatted(T&& value) noexcept {
        return std::forward<T>(value);
    }

    template <typename CharT, typename Traits>
    void validate_next(std::basic_istream<CharT, Traits>& is, CharT expected) {
        CharT actual{};
        is >> actual;
        if (actual != expected) {
            is.setstate(std::basic_istream<CharT, Traits>::failbit);
        }
    }

    template <typename CharT, typename Traits>
    struct print_impl {
        std::basic_ostream<CharT, Traits>& os;
        std::size_t n_args;

        template <typename T>
        void operator()(T&& value) {
            os << formatted(std::forward<T>(value));
            if (--n_args) {
                os << ", ";
            }
        }
    };

    template <typename CharT, typename Traits>
    struct read_impl {
        std::basic_istream<CharT, Traits>& is;
        std::size_t n_args;

        template <typename T>
        void operator()(T&& value) {
            is >> formatted(std::forward<T>(value));
            if (--n_args) {
                validate_next(is, ',');
            }
        }
    };

} //namespace flow::tul::detail