#pragma once

#include <ostream>
#include <sstream>

namespace flow::tul::detail {

    template <typename String, typename CharT>
    struct quoted_string {
        String str;
        CharT delim;
        CharT escape;

        constexpr quoted_string(String s, CharT delim, CharT escape) : str(std::forward<String>(s)), delim(delim), escape(escape) {}

        quoted_string(const quoted_string&) = delete;
    };

    template <typename String, typename CharT, typename Traits>
    std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const quoted_string<String, CharT>& view) {
        using string_t = std::decay_t<String>;

        os << view.delim;

        std::size_t i = 0;
        const std::array<CharT, 2> to_escape{view.delim, view.escape};

        while (i < view.str.size()) {
            std::size_t j = view.str.find_first_of(to_escape.data(), i);
            if (j == string_t::npos) {
                os << view.str.substr(i);
                break;
            }
            else {
                os << view.str.substr(i, j - i);
                os << view.escape;
                os << view.str[j];
            }
            i = j + 1;
        }

        os << view.delim;

        return os;
    }

} // namespace flow::tul::detail
