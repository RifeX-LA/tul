#pragma once

#define MIN_STDCPP_VER(ver) (defined(_MSVC_LANG) && _MSVC_LANG >= ver || __cplusplus >= ver)

#include <flow/tul/io.hpp>

#if MIN_STDCPP_VER(202002L) //C++20
    #include <flow/tul/aggregate_size.hpp>
#endif //C++20