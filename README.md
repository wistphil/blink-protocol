# blink-protocol
This repository contains a work in progress, incomplete implementation of the blink native protocol. The blink native protocol defines a serialization format and a schema language for describing message structure. The main advantage of the blink native protocol is that there is no encoding or decoding phase. Messages are constructed with a fixed-width and a dynamic portion. All fields have a slot in the fixed-width section which allows for their offsets to be calculated in constant time. The tradeoff is that all fields must have space reserved for them in every message, including optional fields, which results in larger message sizes. So, the blink native protocol is ideal for smaller message sizes with less optional fields or in situations where larger buffer sizes and bandwidth are not an issue.

## Build with CMake, Ninja and vcpkg
```
git clone https://github.com/wistphil/blink-protocol.git
cd blink-protocol
mkdir build
cmake --preset linux -DCMAKE_TOOLCHAIN_FILE="path_to_vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build build/
```
## Quick Start
1. Create a blink message schema. In this example the schema is located In a file named ExampleSchema.blink:
```
namespace example

Transaction ->
    i32 timestamp,
    f64 price,
    string description?

Transactions/1 ->
    string owner,
    string (10) account,
    Transaction [] transactions
```
2. Use `blinkc` to generate C++ code. It takes two arguments: the schema file and a path to where the generated files will be created.
```
% ./blinkc ExampleSchema.blink ./
```
this will generate two files: ExampleSchema.hpp and ExampleSchema.cpp
```
% ls
ExampleSchema.blink ExampleSchema.cpp   ExampleSchema.hpp
```
3. Include the generated files in your project
```cpp
#include "ExampleSchema.hpp"

#include <iostream>
#include <vector>

auto main() -> int
{
    std::vector<std::uint8_t> data(256);
    example::Transactions transactions(data);
    transactions.set_owner("j.doe");
    transactions.set_account("Chase");

    auto sequence = transactions.init_transactions(3);
    for (std::size_t i{0}; i < sequence.size(); ++i) {
        auto transaction = sequence[i];
        transaction.set_description("ABC");
        transaction.set_price(5.25);
        transaction.set_timestamp(20230212);
    }

    std::cout << transactions << '\n';
    return 0;
}
```
Sample output
```
Transactions/1 (31) ->
    string owner = j.doe
    string (10) account = Chase
    Transaction [] transactions (3) ->
        transactions [0] ->
            i32 timestamp = 20230212
            f64 price = 5.25
            string description? = ABC
        transactions [1] ->
            i32 timestamp = 20230212
            f64 price = 5.25
            string description? = ABC
        transactions [2] ->
            i32 timestamp = 20230212
            f64 price = 5.25
            string description? = ABC
```

