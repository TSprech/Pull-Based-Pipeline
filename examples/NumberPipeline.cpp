#include "PullPipeline.hpp"
// Seperator because the Arduino library doesn't play nice with the standard library
#include <Arduino.h>

// This is an input, it takes in no parameters and returns a value
pipes::PullInput<int> constant_2 = []() { return 2; };

// This is an output, it takes in a parameter (input) and returns nothing
pipes::PullOutput<int> print_int = [](auto input) { Serial.print(input); };

void setup() {
  Serial.begin(9600);
  // clang-format off
    while(!Serial);
  // clang-format on

  print_int << constant_2;  // This should print "2" because 2 is being returned from constant_2 and passed to print_int which prints it
}

void loop() {
}