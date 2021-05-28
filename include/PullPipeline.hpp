/**
 * @author TSprech
 * @date 5/26/2021
 * @brief Contains the pipes namespace and all types/operators that go along with it.
 * @note Vocabulary used is as follows:
 * A fitting is a lambda in a pipeline (eg a @ref PullInput, PullOutput, PullFilter)
 * A pipe is something that connects two fittings (eg the << operators)
 * A pipeline is a series of fittings and pipes that is at minimum an input, pipe, and output
 * An adapter is a set of fittings that take a 'thing' (object, library, etc) and gives it an interface that works well with a pipeline
 */

#ifndef PULLPIPELINE_HPP
#define PULLPIPELINE_HPP

#include <functional>

namespace pipes {
  /**
     * @brief An input is just a lambda that takes in no parameters (values can be captured by the lambda) and returns a value, the start of a pipeline.
     * @tparam Output_Type The variable type that the input lambda will return (pass) to the preceeding fitting, it MUST match whatever the preceeding fitting's Input_Type is.
     */
  template <typename Output_Type>
  struct PullInput : std::function<Output_Type()> {  // These need to be strongly typed in order for the operators to overload correctly
    using std::function<Output_Type()>::function;    // Expose the constructor !NOTE: There is a better way to do this I just am not sure what it is (maybe Boost::Strong type?)
  };

  /**
     * @brief An output is a lambda that takes in values passed to it (via parameters) and returns nothing as any external variable assignment happens inside the lambda.
     * @tparam Input_Type The variable type that the input lambda will take in as a parameter from the following fitting, it MUST match whatever the following fitting's Output_Type is.
     */
  template <typename... Input_Type>  // This is variadic as an output does not just take in values, it takes in all the functions that come after it up until an input
  struct PullOutput : std::function<void(Input_Type... input)> {
    using std::function<void(Input_Type... input)>::function;
  };

  /**
     * @brief A filter is a lambda that takes in a value and returns a value, doing something to that value in between.
     * @tparam Output_Type The variable type that the input lambda will return (pass) to the preceeding fitting, it MUST match whatever the preceeding fitting's Input_Type is.
     * @tparam Input_Type The variable type that the input lambda will take in as a parameter from the following fitting, it MUST match whatever the following fitting's Output_Type is.
     * @note Output_Type and Input_Type can be different types (eg float and int) just as long as they match the preceeding and following fitting's types.
     */
  template <typename Output_Type, typename... Input_Type>
  struct PullFilter : std::function<Output_Type(Input_Type... input)> {
    using std::function<Output_Type(Input_Type... input)>::function;
  };

  /**
     * @brief This operator can be thought of the 'base case' of a pipeline, once there is only one output lambda (detailed in the next operator) and a single input 
     * lambda, the output lambda calls the input lambda to start the pipeline processing.
     * @tparam Output_Type Allows for a PullInput of any type of be the start of a pipeline, will match whatever the type of the PullInput lambda is.
     * @tparam Input_Type This variadic template allows for the aggregated PullOutput that contains any number of other lambdas to be evaluated.
     * @param output The PullOutput lambda that contains any number (including 0) of PullFilter lambdas that the returned value of @ref input will be passed.
     * @param input The PullInput lambda that returns a value which is passed to @ref output.
     */
  template <typename Output_Type, typename... Input_Type>
  void operator<<(PullOutput<Input_Type...> output, PullInput<Output_Type> input) {
    output(input());  // Call all lambdas starting with the input | eg: [simple] output_lambda(input_lambda())   [complex] output_lambda(filter_1(filter_2(filter_3(input_lambda()))));
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

  /**
     * @brief This operator allows for a @ref PullFilter to be connected to a @ref PullOutput and for the output lambda to call the filter lambda.
     * @tparam Output_Type The shared type between the @ref PullOutput and @ref PullInput which will also be the type that the returned output lambda will take in.
     * @tparam Input_Type This variadic template allows for the filter to take in any number of other filters that follow it.
     * @param output The PullOutput lambda that contains any number (including 0) of PullFilter lambdas.
     * @param filter The PullFilter lambda that will be combined with @ref output to create a single @ref PullOutput lambda.
     * @returns PullOutput<Output_Type> A new @ref PullOutput lambda which combines the @ref output and @ref filter into a single lambda, with the @ref output calling the @ref filter.
     */
  template <typename Output_Type, typename... Input_Type>
  PullOutput<Output_Type> operator<<(PullOutput<Output_Type> output, PullFilter<Output_Type, Input_Type...> filter) {
    return [output, filter](auto input) {  // input is all the fittings that are yet to come
      output(filter(input));               // Generate the PullOutput lambda that calls the filter on whatever fittings come in later down the pipeline
    };
  }
}  // namespace pipes

#endif  // PULLPIPELINE_HPP