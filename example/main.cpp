#include <iostream>
#include <tuple>
#include "utilities.hpp"

struct MyStruct {
    std::string name;
    int age;
    int some_int_value;

    friend std::ostream& operator << (std::ostream& os, const MyStruct& s) {
        return os << s.name << ' ' << s.age << ' ' << s.some_int_value;
    }
};

using cpp::operator <<;
using cpp::operator >>;

int main() {
    std::tuple<std::string, int, int> tuple1{"Peter", 10, 20};
    auto s = cpp::to_struct<MyStruct>(tuple1);

    std::tuple<std::string, int> tuple2;
    std::cin >> tuple2;

    std::cout << "Tuple1: " << tuple1 << std::endl;
    std::cout << "Tuple2: " << tuple2 << std::endl;
    std::cout << "MyStruct: " << s << std::endl;

    return 0;
}
