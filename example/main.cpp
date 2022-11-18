#include <iostream>
#include <flow/tul.hpp>

struct Person {
    std::string name;
    int age;
    double weight;

    friend std::ostream& operator<<(std::ostream& os, const Person& s) {
        return os << s.name << ' ' << s.age << ' ' << s.weight;
    }
};

void mult_by_3(int& n) {
    n *= 3;
}

void get_struct_from_tuple() {
    std::tuple<std::string, int, double> tuple1{"Peter", 10, 20};
    auto p = flow::tul::from_tuple<Person>(tuple1);
    std::cout << "Person: " << p << std::endl;
}

void constructed_from_cin() {
    std::tuple<std::string, int> tuple;
    std::cout << "Enter data for std::tuple<std::string, int>:\n";
    std::cin >> flow::tul::io(tuple);
    std::cout << "Constructed from std::cin: " << flow::tul::io(tuple) << std::endl;
}

void after_mult_by_3() {
    std::tuple<int, int, int> tuple{1, 2, 3};
    flow::tul::for_each(tuple, mult_by_3);
    std::cout << "After mult every element by 3: " << flow::tul::io(tuple) << std::endl;
}

int main() {
    get_struct_from_tuple();
    constructed_from_cin();
    after_mult_by_3();

    std::cout << "Person fields count: " << flow::tul::aggregate_size_v<Person> << std::endl;

    return 0;
}
