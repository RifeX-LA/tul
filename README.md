# Overview
This is the header only library to work with aggregate types and tuple-like objects (`std::tuple`, `std::pair` and `std::array`).

Features:
1. Get struct from tuple-like object 
    ```c++
    struct Person {
        std::string name;
        int age;
        double weight;
    };
    
    ...
    
    std::tuple<std::string, int, double> tuple {"Peter", 10, 20};
    Person person = tutils::from_tuple<Person>(tuple);
    ```
   `person` will contain values `Peter`, `10`, `20`


2. Write tuple-like objects to ostream
   ```c++
   using tutils::operator <<;

   std::tuple<std::string, int, double> tuple {"Peter", 10, 20};
   std::cout << tuple;
   ```
   Output
   ```
   {"Peter", 10, 20}
   ```
   
3. Put values from istream to tuple-like object
   ```c++
   using tutils::operator <<;
   using tutils::operator >>;
   
   std::tuple<std::string, int> tuple;
   std::cin >> tuple;
   std::cout << tuple;
   ```
   Input
   ```
   Hello 5
   ```
   
   Output
   
   ```
   {"Hello", 5}
   ```
   
4. Get aggregate type fields count (*since C++20*)
   ```c++
   std::cout << tutils::aggregate_size_v<Person>;
   or
   std::cout << tutils::aggregate_size<Person>::value;
   ```
   Output
   ```
   3
   ```
   
5. For each cycle for tuple-like object elements
   ```c++
   using tutils::operator <<;
   
   void triple(int& n) {
        n *= 3;
   } 
   
   ...
   
   std::tuple<int, int, int> tuple {1, 2, 3};
   tutils::for_each(tuple, triple);
   std::cout << tuple;
   ```
   
   Output
   ```
   {3, 6, 9}
   ```

# Usage
Put the headers `utilities.hpp` and `aggregate_size.hpp` to your project. Include the header `utilities.hpp`. Nothing to compile

# Requirements
C++17 (C++20 for `tutils::aggregate_size`)
