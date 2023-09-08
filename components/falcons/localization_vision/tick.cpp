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
    int error_value = -1;
    //MRA::Logging::LogTick scoped(timestamp, input, params, &state, &output, &local, &error_value);

    // user implementation goes here

    try
    {
        // setup
        // TODO: how expensive is it to reconstruct everything each tick? we could use static data to improve performance at the cost of state observability/testability
        Solver solver;
        solver.configure(params);
        solver.setState(state);

        // run
        solver.setInput(input);
        error_value = solver.run();

        // store output
        output.CopyFrom(solver.getOutput());
        local.CopyFrom(solver.getDiagnostics());
        state.CopyFrom(solver.getState());
    }
    catch (const std::exception& e)
    {
        std::cerr << "ERROR: Caught a standard exception: " << e.what() << std::endl;
        throw;
    }
    catch (...)
    {
        std::cerr << "ERROR: Caught an unknown exception." << std::endl;
    }

    return error_value;
}

