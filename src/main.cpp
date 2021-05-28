#include <tuple>

#include "PullPipeline.hpp"
// Seperator because the Arduino library doesn't play nice with the standard library
#include <Arduino.h>

  pipes::PullInput<int> input_constant = []() { return 4; };

  pipes::PullFilter<int, int> add_2 = [](auto input) { return input + 2; };

  pipes::PullOutput<int> print_value = [](auto input) { Serial.print(input); };



void setup() {


  print_value << add_2 << input_constant;



}

void loop() {
}