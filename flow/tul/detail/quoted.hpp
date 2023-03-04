#pragma once

#include <flow/tul/detail/type_traits.hpp>
#include <flow/tul/detail/quoted_char.hpp>
#include <flow/tul/detail/quoted_string.hpp>

#include <boost/io/quoted.hpp>

namespace flow::tul::detail {

    template<typename CharT, typename Traits, typename Alloc>
    auto quoted(std::basic_string<CharT, Traits, Alloc>& s) noexcept {
        return boost::io::quoted(s);
    }

    template<typename CharT, typename Traits, typename Alloc>
    auto quoted(const std::basic_string<CharT, Traits, Alloc>& s) noexcept {
        return boost::io::quoted(s);
    }

    template<typename CharT>
    auto quoted(const CharT* s) noexcept {
        return boost::io::quoted(s);
    }

    template<typename CharT, typename Traits>
    auto quoted(std::basic_string_view<CharT, Traits> sv) noexcept {
        return quoted_string<std::basic_string_view<CharT, Traits>, CharT>{sv, '"', '\\'};
    }

    template<typename CharT, typename = std::enable_if_t<detail::is_char_v<std::decay_t<CharT>>>>
    auto quoted(CharT&& c) noexcept {
        return quoted_char<CharT&&, std::decay_t<CharT>>{std::forward<CharT>(c), '\''};
    }

    template<typename T, typename = std::enable_if_t<!detail::is_char_v<std::decay_t<T>>>>
    decltype(auto) quoted(T&& t) noexcept {
        return std::forward<T>(t);
    }

} // namespace flow::tul::detail
