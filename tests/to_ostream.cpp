#include <fstream>
#include <gtest/gtest.h>
#include <flow/tul.hpp>

template <typename Tuple>
std::string make_string_from_tuple(const Tuple& tuple) {
    std::ostringstream oss;
    oss << flow::tul::io(tuple);
    return oss.str();
}

template <typename Tuple>
std::string make_string_from_tuple(const Tuple& tuple, std::string_view sep) {
    std::ostringstream oss;
    oss << flow::tul::io(tuple, sep);
    return oss.str();
}

TEST(to_ostream, EmptyTuple) {
    std::tuple<> tuple;
    EXPECT_EQ(make_string_from_tuple(tuple), "()");
}

TEST(to_ostream, IntTuple) {
    std::tuple<int> tuple{42};
    EXPECT_EQ(make_string_from_tuple(tuple), "(42)");
}

TEST(to_ostream, DoubleIntCharTuple) {
    std::tuple<double, int, char> tuple{10.25, 42, 'c'};
    EXPECT_EQ(make_string_from_tuple(tuple), "(10.25, 42, 'c')");
}

TEST(to_ostream, StringTuple) {
    std::tuple<std::string> tuple{"Hello world"};
    EXPECT_EQ(make_string_from_tuple(tuple), "(\"Hello world\")");
}

TEST(to_ostream, IntStringDoubleTuple) {
    std::tuple<int, std::string, double> tuple{42, "Hello world", 10.25};
    EXPECT_EQ(make_string_from_tuple(tuple), "(42, \"Hello world\", 10.25)");
}

struct UserStruct {
    int a;
    std::string str;

    friend std::ostream& operator<<(std::ostream& os, const UserStruct& us) {
        return os << us.a << ' ' << us.str;
    }
};

TEST(to_ostream, UserStructTuple) {
    std::tuple<UserStruct> tuple{{42, "Hello world"}};
    EXPECT_EQ(make_string_from_tuple(tuple), "(42 Hello world)");
}

TEST(to_ostream, CDynamicArrayTuple) {
    const char* str = "Hello world";
    std::tuple<const char*> tuple{str};
    EXPECT_EQ(make_string_from_tuple(tuple), "(\"Hello world\")");
}

TEST(to_ostream, IntStringPair) {
    std::pair<int, std::string> pair{42, "Hello world"};
    EXPECT_EQ(make_string_from_tuple(pair), "(42, \"Hello world\")");
}

TEST(to_ostream, IntStdArray) {
    std::array<int, 4> array{1, 2, 3, 4};
    EXPECT_EQ(make_string_from_tuple(array), "(1, 2, 3, 4)");
}

TEST(to_ostream, StringStdArray) {
    std::array<std::string, 2> array{"Hello", "World"};
    EXPECT_EQ(make_string_from_tuple(array), "(\"Hello\", \"World\")");
}

TEST(to_ostream, WithCustomSeparator1) {
    std::tuple<int, std::string, char> tuple{42, "Hello world", 'c'};
    EXPECT_EQ(make_string_from_tuple(tuple, " -- "), "42 -- \"Hello world\" -- 'c'");
}

TEST(to_ostream, WithCustomSeparator2) {
    std::tuple<int, std::string, char> tuple{42, "Hello world", 'c'};
    EXPECT_EQ(make_string_from_tuple(tuple, "*"), "42*\"Hello world\"*'c'");
}

TEST(to_ostream, PassToWostream) {
    std::tuple<int, std::wstring, wchar_t> tuple{42, L"Hello world", 'c'};
   
}
