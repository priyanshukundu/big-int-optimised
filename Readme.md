Readme
# BigInt Library

## Overview

The `BigInt` library provides a class for handling large integers beyond the limits of standard data types in C++. It supports basic arithmetic operations, comparison, and some special functions like factorial, Fibonacci, and Catalan numbers.

## Features

- **Arithmetic Operations**: Addition, subtraction, multiplication, division, and modulus.
- **Increment/Decrement**: Prefix and postfix.
- **Comparison Operations**: Equal, not equal, less than, greater than, less than or equal to, greater than or equal to.
- **Special Functions**: Factorial, nth Fibonacci, nth Catalan, and square root.
- **I/O Operations**: Stream input and output.

## Installation

To use the `BigInt` library, simply include the `BigInt.h` file in your project.

## Usage

### Basic Usage

```cpp
#include "BigInt.h"
#include <iostream>

int main() {
    BigInt a = 12345678901234567890ULL;
    BigInt b = "98765432109876543210";
    BigInt c = a + b;
    std::cout << "a + b = " << c << std::endl;
    return 0;
}

COMPILATION 

g++ -o main main.cpp


This README provides a comprehensive guide to using the `BigInt` library, covering installation, usage, and examples of operations. Adjust it based on your specific project details and preferences.

