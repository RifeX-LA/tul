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
    Person person = cpp::from_tuple<Person>(tuple);
    ```
   `person` will contain values `Peter`, `10`, `20`


2. Write tuple-like objects to ostream
   ```c++
   using cpp::operator <<;

   std::tuple<std::string, int, double> tuple {"Peter", 10, 20};
   std::cout << tuple;
   ```
   Output
   ```
   {"Peter", 10, 20}
   ```
   
3. Put values from istream to tuple-like object
   ```c++
   using cpp::operator <<;
   using cpp::operator >>;
   
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
   std::cout << cpp::aggregate_size_v<Person>;
   or
   std::cout << cpp::aggregate_size<Person>::value;
   ```
   Output
   ```
   3
   ```

# Usage
Include the header `utilities.hpp`. Nothing to compile

# Requirements
C++17 (C++20 for `cpp::aggregate_size`)
