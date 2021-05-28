#include <tuple>
#include "PullPipeline.hpp"
// Seperator because the Arduino library doesn't play nice with the standard library
#include <Arduino.h>

pipes::PullInput<std::tuple<int, float, char>> multiple_values = []() {
  return std::make_tuple(0, 0.0, ' '); // Make a tuple with some uninteresting values
};

pipes::PullFilter<std::tuple<int, float, char>, std::tuple<int, float, char>> assign_float_3_6_int_20 = [](auto input) {
  auto [input_int, input_float, input_char] = input;  // C++17 Structured bindings, unpacks a tuple into named variables
  input_float = 3.6; // Assign the float a value
  input_int = 20; // and the int
  return std::make_tuple(input_int, input_float, input_char); // Then return a tuple with the updated values
};

pipes::PullFilter<std::tuple<int, float, char>, std::tuple<int, float, char>> assign_char_s = [](auto input) {
  auto [input_int, input_float, input_char] = input;
  input_char = 's';
  return std::make_tuple(input_int, input_float, input_char);
};

float output = 0;
// This is an output, it takes in a parameter (input) and returns nothing
pipes::PullOutput<std::tuple<int, float, char>> print_values = [](auto input) {
  auto [input_int, input_float, input_char] = input;
  Serial.println(input_float);
  Serial.println(input_int);
  Serial.println(input_char);
};

void setup() {
  Serial.begin(9600);
  // clang-format off
    while(!Serial);
  // clang-format on

  print_values << assign_char_s << assign_float_3_6_int_20 << multiple_values;  // This should print "2" because 2 is being returned from constant_2 and passed to print_int which prints it
}

void loop() {
}