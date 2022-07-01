#include <iostream>
#include <tuple>
#include <utilities.hpp>

struct Person {
    std::string name;
    int age;
    double weight;

    friend std::ostream& operator << (std::ostream& os, const Person& s) {
        return os << s.name << ' ' << s.age << ' ' << s.weight;
    }
};

using cpp::operator <<;
using cpp::operator >>;

int main() {
    std::tuple<std::string, int, double> tuple1{"Peter", 10, 20};
    auto p = cpp::from_tuple<Person>(tuple1);

    std::tuple<std::string, int> tuple2;
    std::cin >> tuple2;

    std::cout << "Tuple1: " << tuple1 << std::endl;
    std::cout << "Tuple2: " << tuple2 << std::endl;
    std::cout << "Person: " << p << std::endl;

    std::cout << "Person fields count: " << cpp::aggregate_size_v<Person> << std::endl;

    return 0;
}
