# Pull-Based-Pipeline
![Travis CI](https://img.shields.io/travis/com/TSprech/Pull-Based-Pipeline?logo=Travis-CI&logoColor=FFFFFF&style=flat&link=https://travis-ci.com/github/TSprech/Pull-Based-Pipeline) ![CPlusPlus](https://img.shields.io/badge/Version-C%2B%2B17-blue?logo=c%2B%2B&style=flat) ![GitHub](https://img.shields.io/github/license/TSprech/Pull-Based-Pipeline?color=%23D22128&logo=apache)

Offering a different way to create Arduino (and C++ in general) programs revolving around creating modular, easy to understand data pipelines as opposed to the typical layered approach often used.

- [Pull-Based-Pipeline](#pull-based-pipeline)
  - [Tested boards](#tested-boards)
  - [Good things to know going into this](#good-things-to-know-going-into-this)
  - [Things you need to know to understand how it operates](#things-you-need-to-know-to-understand-how-it-operates)
    - [PlatformIO Setup](#platformio-setup)
  - [License](#license)

## Tested boards
- Adafruit Grand Central M4
- Adafruit Metro M0

*Note that some boards, (such as the Sparkfun SAMD21 Dev Board) do not work with this system, I am not entirely sure why, as the Metro M0 and SAMD21 Dev share the same microcontroller. The SAMD21 Dev hard faults when this runs, which leads me to believe it is something with the bootloader. When running in a desktop environment, it works consistently.*

## Good things to know going into this
- Understanding of C++ lambdas
  - This library uses C++14 style lambdas in all the examples, which primarily boils down to using `auto` for parameters to make life easier.
  - Lambdas in general should be understood, the lambdas in this are simple, but understanding the syntax and how they operate [capture clauses] and (parameter deduction) is important.
- Understanding of how the `auto` keyword works. It is pretty simple, but used in many places in pipelines to avoid type mismatching. This is especially helpful with input parameters as the input type is already defined in the fitting's type, so `auto` helps prevent mismatch.
## Things you need to know to understand how it operates
- Variadic templates
  - These are the heart of this library, they are used to essentially allow a lambda to take in a value returned from a lambda that has not been parsed yet
- Operator overloading
  - To get the easy to use, easy to understand `<<` syntax, the `<<` operator is overload just like in the C++ osstream library.
- Lots about lambdas
  - Lambdas are used as both variables and named functions, and understanding how they can be passed and called is vital.

### PlatformIO Setup
You should make sure you have `build_unflags = -std=gnu++11` and `build_flags = -std=c++17` in your platformio.ini file to make use of the newer standard features.

## License
[Apache-2.0](https://choosealicense.com/licenses/apache-2.0/)

This software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.