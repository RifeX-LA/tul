#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <flow/tul/aggregate_size.hpp>

struct Empty {};

struct IntField {
    int a;
};

struct CharIntDoubleFields {
    char a;
    int b;
    double c;
};

struct ArrayField {
    char a[100];
};

struct ArrayIntFields {
    char a[100];
    int b;
};

struct IntArrayFields {
    int b;
    char a[60];
};

struct StringVectorFields {
    std::string a;
    std::vector<std::string> b;
};

struct Mixed1 {
    char a[100];
    std::vector<int> b;
    double c;
    std::string d;
    char e;
};

struct Mixed2 {
    std::thread a;
    double b;
    std::mutex c;
};

static_assert(flow::tul::aggregate_size_v<Empty> == 0);
static_assert(flow::tul::aggregate_size_v<IntField> == 1);
static_assert(flow::tul::aggregate_size_v<CharIntDoubleFields> == 3);
static_assert(flow::tul::aggregate_size_v<ArrayField> == 1);
static_assert(flow::tul::aggregate_size_v<ArrayIntFields> == 2);
static_assert(flow::tul::aggregate_size_v<IntArrayFields> == 2);
static_assert(flow::tul::aggregate_size_v<StringVectorFields> == 2);
static_assert(flow::tul::aggregate_size_v<Mixed1> == 5);
//static_assert(flow::tul::aggregate_size_v<Mixed2> == 2); --> NOT WORKING BECAUSE OF std::thread
