# Overview
This is the header only library to work with tuple-like objects (including `std::array`) \
Features:
1. Get struct from tuple-like object 
    ```c++
    struct MyStruct {
        std::string name;
        int age;
        int some_int_value;
    };
    
    ...
    
    std::tuple<std::string, int, int> tuple {"Peter", 10, 20};
    MyStruct my_struct = cpp::to_struct<MyStruct>(tuple);
    ```
   `my_struct` will contain values `"Peter"`, `10`, `20`

2. Write tuple-like objects to ostream
   ```c++
   using cpp::operator <<;

   std::tuple<std::string, int, int> tuple {"Peter", 10, 20};
   std::cout << tuple;
   ```
   Output
   ```
   {Peter, 10, 20}
   ```
   
3. Put values from istream to tuple-like object
   ```c++
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
   {Hello, 5}
   ```

# Usage
Include the header `utilities.hpp`. Nothing to compile

# Requirements
C++17
