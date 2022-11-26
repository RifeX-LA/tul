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
    Person person = flow::tul::from_tuple<Person>(tuple);
    ```
   `person` will contain values `Peter`, `10`, `20`


2. Write tuple-like objects to ostream
   ```c++
   std::tuple<std::string, int, double> tuple {"Peter", 10, 20};
   std::cout << flow::tul::io(tuple);
   ```
   Output
   ```
   {"Peter", 10, 20}
   ```
   
3. Put values from istream to tuple-like object
   ```c++
   std::tuple<std::string, int> tuple;
   std::cin >> flow::tul::io(tuple);
   std::cout << flow::tul::io(tuple);
   ```
   Input
   ```
   {"Hello", 5}
   ```
   
   Output
   
   ```
   {"Hello", 5}
   ```
   
4. For each cycle for tuple-like object elements
   ```c++
   void mult_by_3(int& n) {
        n *= 3;
   } 
   
   ...
   
   std::tuple<int, int, int> tuple {1, 2, 3};
   flow::tul::for_each(tuple, mult_by_3);
   std::cout << flow::tul::io(tuple);
   ```
   
   Output
   ```
   {3, 6, 9}
   ```


5. Get aggregate type fields count (*since C++20*)
   ```c++
   std::cout << flow::tul::aggregate_size_v<Person>;
   ```
   or
   ```c++
   std::cout << flow::tul::aggregate_size<Person>::value;
   ```
   Output
   ```
   3
   ```

# Usage
Put directory `flow` to your project and include `tul.hpp` header. Nothing to build

# Tests
`googletest` is required for run tests.

To run tests follow this steps:

1. If you already have `googletest` downloaded and built, then you can just put it to the `tests` directory
and go to the next step.

   If you don't have `googletest`, then clone repository using this command

   ```
   git clone --recurse-submodules https://github.com/RifeX-LA/Tuple-Utilities.git
   ```
   
   or if you already cloned this repository you need to clone googletest repository to the `tests` directory using this command
   ```
   git clone https://github.com/google/googletest.git
   ```

2. To build tests in the `tests` directory:

   ```
   mkdir build
   cd build
   cmake ..
   ```

   On *nix system run
   
   ```
   make
   ```
   
   On Windows system you can build project using Visual Studio


3. Run `Google_Tests_run` file to start the tests

# Requirements
C++17 (C++20 for `flow::tul::aggregate_size`) \
`googletest` for run tests
