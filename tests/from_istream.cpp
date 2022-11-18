#include <gtest/gtest.h>
#include <flow/tul.hpp>

template <typename Tuple>
Tuple constructed_from_istream(const Tuple& tuple) {
    std::stringstream ss;
    ss << flow::tul::io(tuple);
    Tuple res{};
    ss >> flow::tul::io(res);

    return res;
}

TEST(from_istream, EmptyTuple) {
    std::tuple<> tuple{};
    EXPECT_EQ(constructed_from_istream(tuple), tuple);
}

TEST(from_istream, IntTuple) {
    std::tuple<int, int> tuple{1, 2};
    EXPECT_EQ(constructed_from_istream(tuple), tuple);
}

TEST(from_istream, IntStringTuple) {
    std::tuple<int, std::string> tuple{42, "Hello"};
    EXPECT_EQ(constructed_from_istream(tuple), tuple);
}

TEST(from_istream, IntTwoWordStringTuple) {
    std::tuple<int, std::string> tuple{42, "Hello world"};
    EXPECT_EQ(constructed_from_istream(tuple), tuple);
}

TEST(from_istream, ManyFieldsTuple) {
    std::tuple<std::string, char, int, double> tuple{"Hello", 'c', 42, 10.25};
    EXPECT_EQ(constructed_from_istream(tuple), tuple);
}

struct UserStruct {
    int a;
    std::string str;

    friend std::ostream& operator << (std::ostream& os, const UserStruct& us) {
        return os << us.a << ' ' << std::quoted(us.str);
    }

    friend std::istream& operator >> (std::istream& is, UserStruct& us) {
        return is >> us.a >> std::quoted(us.str);
    }

    bool operator == (const UserStruct& other) const noexcept {
        return a == other.a && str == other.str;
    }
};

TEST(from_istream, UserStructTuple) {
    std::tuple<int, UserStruct, char> tuple{42, {10, "Hello"}, 'c'};
    EXPECT_EQ(constructed_from_istream(tuple), tuple);
}