# BigInt

BigInt is a lightweight C library for handling arbitrary-precision integers. It provides basic arithmetic operations and comparison functions for large numbers.

## Features
- Arithmetic operations: addition (+), subtraction (-), multiplication (*)
- Comparison operations for big integers
- Written in C for performance and portability

## Dependencies
- [Unity Framework](https://github.com/ThrowTheSwitch/Unity) (included as a Git submodule) is required for running tests.

## Building the Project
The project uses a Makefile for building. Available commands:
- `make debug`: Build the debug version
- `make release`: Build the release version
- `make install`: Install the library to `/usr/local/`

## Examples
Example usage of the library can be found in the `examples/` directory.

## License
This project is licensed under the MIT License.
