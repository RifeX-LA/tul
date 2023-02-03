#pragma once

#include <flow/tul/detail/io_view.hpp>

namespace flow::tul {

    /**
     * @brief returns wrapper, that can be used with iostreams
     * @param tuple tuple-like object to wrap
     */
    template <typename Tuple>
    [[nodiscard]] constexpr auto io(Tuple&& tuple) noexcept {
        return detail::io_view<Tuple&&>{std::forward<Tuple>(tuple)};
    }

    /**
     * @brief returns wrapper with given sep, that can be used with iostreams
     * @param tuple tuple-like object to wrap
     */
    template <typename Tuple>
    [[nodiscard]] constexpr auto io(Tuple&& tuple, std::string_view sep) noexcept {
        return detail::sep_io_view<Tuple&&>{std::forward<Tuple>(tuple), sep};
    }

    /**
     * @brief returns wrapper with given sep, that can be used with iostreams
     * @param tuple tuple-like object to wrap
     */
    template <typename Tuple>
    [[nodiscard]] constexpr auto io(Tuple&& tuple, std::wstring_view sep) noexcept {
        return detail::sep_io_view<Tuple&&, wchar_t>{std::forward<Tuple>(tuple), sep};
    }

#ifdef __cpp_lib_char8_t
    /**
     * @brief returns wrapper with given sep, that can be used with iostreams
     * @param tuple tuple-like object to wrap
     */
    template <typename Tuple>
    [[nodiscard]] constexpr auto io(Tuple&& tuple, std::u8string_view sep) noexcept {
        return detail::sep_io_view<Tuple&&, char8_t>{std::forward<Tuple>(tuple), sep};
    }
#endif // __cpp_lib_char8_t

    /**
     * @brief returns wrapper with given sep, that can be used with iostreams
     * @param tuple tuple-like object to wrap
     */
    template <typename Tuple>
    [[nodiscard]] constexpr auto io(Tuple&& tuple, std::u16string_view sep) noexcept {
        return detail::sep_io_view<Tuple&&, char16_t>{std::forward<Tuple>(tuple), sep};
    }

    /**
     * @brief returns wrapper with given sep, that can be used with iostreams
     * @param tuple tuple-like object to wrap
     */
    template <typename Tuple>
    [[nodiscard]] constexpr auto io(Tuple&& tuple, std::u32string_view sep) noexcept {
        return detail::sep_io_view<Tuple&&, char32_t>{std::forward<Tuple>(tuple), sep};
    }

    /**
     * @brief returns wrapper with given sep, that can be used with iostreams
     * @param tuple tuple-like object to wrap
     */
    template <typename Tuple, typename CharT, typename Traits>
    [[nodiscard]] constexpr auto io(Tuple&& tuple, std::basic_string_view<CharT, Traits> sep) noexcept {
        return detail::sep_io_view<Tuple&&, CharT, Traits>{std::forward<Tuple>(tuple), sep};
    }

} // namespace flow::tul
