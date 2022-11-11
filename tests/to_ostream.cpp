#include <fstream>
#include <gtest/gtest.h>
#include <flow/tul/functions.hpp>

template <typename Tuple>
std::string get_tuple_string(const Tuple& tuple) {
    using flow::tul::operator<<;

    std::ostringstream oss;
    oss << tuple;
    return oss.str();
}

TEST(to_ostream, EmptyTuple) {
    std::tuple<> tuple;
    EXPECT_EQ(get_tuple_string(tuple), "{}");
}

TEST(to_ostream, IntTuple) {
    std::tuple<int> tuple{42};
    EXPECT_EQ(get_tuple_string(tuple), "{42}");
}

TEST(to_ostream, DoubleIntCharTuple) {
    std::tuple<double, int, char> tuple{10.25, 42, 'c'};
    EXPECT_EQ(get_tuple_string(tuple), "{10.25, 42, c}");
}

TEST(to_ostream, StringTuple) {
    std::tuple<std::string> tuple{"Hello world"};
    EXPECT_EQ(get_tuple_string(tuple), "{\"Hello world\"}");
}

TEST(to_ostream, IntStringDoubleTuple) {
    std::tuple<int, std::string, double> tuple{42, "Hello world", 10.25};
    EXPECT_EQ(get_tuple_string(tuple), "{42, \"Hello world\", 10.25}");
}

struct UserStruct {
    int a;
    std::string str;

    friend std::ostream& operator << (std::ostream& os, const UserStruct& us) {
        return os << us.a << ' ' << us.str;
    }
};

TEST(to_ostream, UserStructTuple) {
    std::tuple<UserStruct> tuple{{42, "Hello world"}};
    EXPECT_EQ(get_tuple_string(tuple), "{42 Hello world}");
}

TEST(to_ostream, CDynamicArrayTuple) {
    const char* str = "Hello world";
    std::tuple<decltype(str)> tuple{str};
    EXPECT_EQ(get_tuple_string(tuple), "{\"Hello world\"}");
}

TEST(to_ostream, IntStringPair) {
    std::pair<int, std::string> pair(42, "Hello world");
    EXPECT_EQ(get_tuple_string(pair), "{42, \"Hello world\"}");
}

TEST(to_ostream, IntStdArray) {
    std::array<int, 4> array{1, 2, 3, 4};
    EXPECT_EQ(get_tuple_string(array), "{1, 2, 3, 4}");
}

TEST(to_ostream, StringStdArray) {
    std::array<std::string, 2> array{"Hello", "World"};
    EXPECT_EQ(get_tuple_string(array), "{\"Hello\", \"World\"}");
}
