#include <iostream>
#include <flow/tul.hpp>

struct Person {
    std::string name;
    int age;
    double weight;

    friend std::ostream& operator << (std::ostream& os, const Person& s) {
        return os << s.name << ' ' << s.age << ' ' << s.weight;
    }
};

using flow::tul::operator <<;
using flow::tul::operator >>;

void triple(int& n) {
    n *= 3;
}

int main() {
    std::tuple<std::string, int, double> tuple1{"Peter", 10, 20};
    auto p = flow::tul::from_tuple<Person>(tuple1);

    std::tuple<std::string, int> tuple2;
    std::cin >> tuple2;

    std::tuple<int, int, int> tuple3{1, 2, 3};
    flow::tul::for_each(tuple3, triple);

    std::cout << "Tuple1: " << tuple1 << std::endl;
    std::cout << "Tuple2: " << tuple2 << std::endl;
    std::cout << "Tuple3: " << tuple3 << std::endl;
    std::cout << "Person: " << p << std::endl;

    std::cout << "Person fields count: " << flow::tul::aggregate_size_v<Person> << std::endl;

    return 0;
}
