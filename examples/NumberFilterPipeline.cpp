#include "PullPipeline.hpp"
// Seperator because the Arduino library doesn't play nice with the standard library
#include <Arduino.h>

int input = 3;
// This is an input, it takes in no parameters and returns a value
pipes::PullInput<int> constant = []() { return input; };

pipes::PullFilter<float, int> divide_by_2 = [](auto input) { return static_cast<float>(input) / 2.0; };

float output = 0;
// This is an output, it takes in a parameter (input) and returns nothing
pipes::PullOutput<float> assign_float = [](auto input) { output = input; };

auto pipeline = assign_float << divide_by_2 << constant;  // This takes input, casts it and divides it by 2, then assigns the result to output

void setup() {
  Serial.begin(9600);
  // clang-format off
    while(!Serial);
  // clang-format on

  pipeline();
  Serial.print(output);  // This should print "1.50"
}

void loop() {
}