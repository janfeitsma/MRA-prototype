// this file was produced by MRA-codegen.py from template_tick.cpp
// with the intent of letting user add the implementation here

// generated component header:
#include "FalconsVelocityControl.hpp"

using namespace MRA;

// internals
#include "internal/include/VelocityControl.hpp"


int FalconsVelocityControl::FalconsVelocityControl::tick
(
    google::protobuf::Timestamp timestamp,   // absolute timestamp
    InputType  const           &input,       // input data, type generated from Input.proto
    ParamsType const           &params,      // configuration parameters, type generated from Params.proto
    StateType                  &state,       // state data, type generated from State.proto
    OutputType                 &output,      // output data, type generated from Output.proto
    LocalType                  &local        // local/diagnostics data, type generated from Local.proto
)
{
    int error_value = 0;
    MRA_LOG_TICK();

    // user implementation goes here

    // relay to internal implementation which is a stripped version of the package `velocityControl` from falcons/code
    // making use of ReflexxesTypeII trajectory generation library
    MRA::internal::FVC::VelocityControl controller;
    controller.data.timestamp = timestamp;
    controller.data.input = input;
    controller.data.config = params;
    controller.data.state = state;
    try
    {
        controller.iterate();
        output = controller.data.output;
        state = controller.data.state;
        local = controller.data.diag;
    }
    catch (const std::exception& e)
    {
		MRA_LOG_ERROR("ERROR: Caught a standard exception: %s", e.what());
        error_value = -1;
    }
    catch (...)
    {
		MRA_LOG_ERROR("ERROR: Caught an unknown exception.");
        error_value = -1;
    }

    return error_value;
}

