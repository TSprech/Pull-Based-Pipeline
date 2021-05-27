/**
 * @author TSprech
 * @date 5/26/2021
 * @brief Contains the pipes namespace and all types/operators that go along with it.
 */

#ifndef PULLPIPELINE_HPP
#define PULLPIPELINE_HPP

#include <functional>

namespace pipes {
// Note the term function is used frequently but in practice all the "functions" are actually lambdas
// An input is just a function that takes in no parameters (values are captured by the lambda) and returns a value, the start of a pipeline
template <typename Output_Type>                    // Output_Type is the type that the input function will pass onto the filter or output (the return value)
struct PullInput : std::function<Output_Type()> {  // These need to be strongly typed in order for the operators to overload correctly
  using std::function<Output_Type()>::function;    // Expose the constructor !NOTE: There is a better way to do this I just am not sure what it is (maybe Boost::Strong type?)
};

// An output is a function that takes in values passed to it (via parameters) and returns nothing as variable assignment happens inside the lambda
template <typename... Input_Type>                               // This is variadic as an output does not just take in values, it takes in all the functions that come after it up until an input
struct PullOutput : std::function<void(Input_Type... input)> {  // Strongly typed
  using std::function<void(Input_Type... input)>::function;     // Expose constructor
};

// A filter is a function that takes in arguments and returns a value, transforming the value in between
template <typename Output_Type, typename... Input_Type>                // Output_Type is just what type the function returns, Input_Type is variadic for the same reason it is in PullOutput, it will take in other functions
struct PullFilter : std::function<Output_Type(Input_Type... input)> {  // Strongly typed
  using std::function<Output_Type(Input_Type... input)>::function;     // Expose constructor
};

// This operator can be thought of the 'base case' of a pipeline, once there is only one output (detailed in the next operator) and an input, the input function is called to start the pipeline processing
template <typename Output_Type, typename... Input_Type>
// Output_Type is the type that is returned from the PullInput, Input_Type is the aggregate of all the functions leading up to the PullOutput
void operator<<(PullOutput<Input_Type...> output, PullInput<Output_Type> input) {  // The left hand side takes in the lambda that will receive the processed value and the right hand is the function that generates the initial pipeline input
  output(input());                                                                 // Call all functions starting with the input | eg: [simple] output_lambda(input_lambda())   [complex] output_lambda(filter_1(filter_2(filter_3(input_lambda()))));
}

// This is the most complex part of the pipeline, this operator generates an output lambda that has a new parameter which is a PullFilter lambda
// The easiest way (not a perfect representation but its a start) to think about it is as follows:
// As the code is parsed left to right, the compiler sees a function that takes in at least 1 parameter but that parameter (filter) relies on things the compiler is not yet aware if
// so we say "just create a new lambda that will call another lambda but don't worry about what that lambda will take in (auto input)" so we essentially have this:
//  output(filter(<and a promise to the compiler that what goes here will come later>
// So this lambda is returned as a new PullOutput which will take in some number of arguments and will now execute output(filter(<argument to come later>))
// As this process is repeated, we end up with a single PullOutput lambda which "contains" all the other filters in the order they were placed in, ready to execute as soon as an argument is given
// Eventually we reach a point where we have output(filter1(filter2(filter3(filter4(<still waiting on arguments> and then the PullInput lambda is reached, which at that point the other operator kicks in
// Keep in mind that output(filter1(filter2(filter3(filter4(<still waiting on arguments> has all been compressed into a single lambda so it is in effect a PullOutput
// The input is given to the aggregate PullOutput via output(input()); and the entire pipeline is executed
// Short version: The ... keep the compiler from complaining because it didn't finish reading the line and allow for lambda chaining
template <typename Output_Type, typename... Input_Type>
PullOutput<Output_Type>
operator<<(PullOutput<Output_Type> output, PullFilter<Output_Type, Input_Type...> filter) {
  return [output, filter](auto input) { output(filter(input)); };
}
}  // namespace pipes

#endif  // PULLPIPELINE_HPP