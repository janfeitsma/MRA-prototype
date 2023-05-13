// this file was produced by MRA-codegen.py from template_tick.cpp
// with the intent of letting user add the implementation here

// generated component header:
#include "FalconsVelocityControl.hpp"

using namespace MRA;

// internals
#include "internal/include/VelocityControl.hpp"


int FalconsVelocityControl::FalconsVelocityControl::tick
(
    double            timestamp,   // simulation timestamp, seconds since start of simulation
    InputType  const &input,       // input data, type generated from Input.proto
    ParamsType const &params,      // configuration parameters, type generated from Params.proto
    StateType        &state,       // state data, type generated from State.proto
    OutputType       &output,      // output data, type generated from Output.proto
    LocalType        &local        // local/diagnostics data, type generated from Local.proto
)
{
    //std::cout << "timestamp: " << timestamp << std::endl;
    //std::cout << "input: " << convert_proto_to_json_str(input) << std::endl;
    //std::cout << "params: " << convert_proto_to_json_str(params) << std::endl;
    //std::cout << "state: " << convert_proto_to_json_str(state) << std::endl;

    int error_value = 0;

    // user implementation goes here

    // relay to internal implementation which is a stripped version of the package `velocityControl` from falcons/code
    // making use of ReflexxesTypeII trajectory generation library
    VelocityControl controller;
    controller.data.timestamp = timestamp;
    controller.data.input = input;
    controller.data.config = params;
    controller.data.state = state;
    //try
    //{
        controller.iterate();
        output = controller.data.output;
        state = controller.data.state;
        local = controller.data.diag;
    //}
    //catch
    //{
    //    error_value = 1;
    //}

    //std::cout << "output: " << convert_proto_to_json_str(output) << std::endl;
    //std::cout << "state: " << convert_proto_to_json_str(state) << std::endl;
    //std::cout << "error: " << error_value << std::endl;

    return error_value;
}

