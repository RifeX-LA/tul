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

void mult_by_3(int& n) {
    n *= 3;
}

int main() {
    std::tuple<std::string, int, double> tuple1{"Peter", 10, 20};
    auto p = flow::tul::from_tuple<Person>(tuple1);

    std::tuple<std::string, int> tuple2;
    std::cin >> flow::tul::io(tuple2);

    std::tuple<int, int, int> tuple3{1, 2, 3};
    flow::tul::for_each(tuple3, mult_by_3);

    std::cout << "Tuple1: " << flow::tul::io(tuple1) << std::endl;
    std::cout << "Tuple2: " << flow::tul::io(tuple2) << std::endl;
    std::cout << "Tuple3: " << flow::tul::io(tuple3) << std::endl;
    std::cout << "Person: " << p << std::endl;

    std::cout << "Person fields count: " << flow::tul::aggregate_size_v<Person> << std::endl;

    return 0;
}
