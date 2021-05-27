#include "PullPipeline.hpp"
// Seperator because the Arduino library doesn't play nice with the standard library
#include <Arduino.h>

pipes::PullInput<int> constant_2 = []() { return 2; };
pipes::PullOutput<int> print_int = [](auto input) { Serial.print(input); };

void setup() {
  Serial.begin(9600);
  // clang-format off
    while(!Serial);
  // clang-format on

  print_int << constant_2; // This should print "2"
}

void loop() {
}