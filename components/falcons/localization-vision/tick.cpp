// this file was produced by MRA-codegen.py from template_tick.cpp
// with the intent of letting user add the implementation here

// generated component header:
#include "FalconsLocalizationVision.hpp"

using namespace MRA;

// custom includes, if any
#include "solver.hpp"
#include "logging.hpp" // TODO: automate, perhaps via generated hpp



int FalconsLocalizationVision::FalconsLocalizationVision::tick
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
    MRA::Logging::LogTick scoped(timestamp, input, params, &state, &output, &local, &error_value);

    // user implementation goes here

    // setup
    // TODO: how expensive is it to reconstruct everything each tick? we could use static data to improve performance at the cost of state observability/testability
    Solver solver;
    solver.configure(params);
    solver.set_state(state);
    solver.determine_reference_floor();

    // run
    solver.set_input(input);
    error_value = solver.run();

    // store output
    output.CopyFrom(solver.get_output());
    local.CopyFrom(solver.get_diagnostics());
    state.CopyFrom(solver.get_state());

    return error_value;
}

