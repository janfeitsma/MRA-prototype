// this file was produced by MRA-codegen.py from template_tick.cpp
// with the intent of letting user add the implementation here

// generated component header:
#include "FalconsLocalizationVision.hpp"

using namespace MRA;

// custom includes, if any
#include "solver.hpp"
#include <fstream>


#define DEBUG

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
#ifdef DEBUG
    std::cout << "timestamp: " << timestamp << std::endl;
    std::cout << "input: " << convert_proto_to_json_str(input) << std::endl;
    std::cout << "params: " << convert_proto_to_json_str(params) << std::endl;
    //std::cout << "state: " << convert_proto_to_json_str(state) << std::endl; // a lot of binary data ...
#endif // DEBUG
    int error_value = 0;

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

#ifdef DEBUG
    std::cout << "output: " << convert_proto_to_json_str(output) << std::endl;
    //std::cout << "state: " << convert_proto_to_json_str(state) << std::endl; // a lot of binary data ...
    std::cout << "state_img: " << state.referencefloor().height() << "x" << state.referencefloor().width() << std::endl;
    std::ofstream dumpState;
    dumpState.open("/tmp/locState.bin");
    state.SerializeToOstream(&dumpState);
    dumpState.close();
    std::cout << "local: " << convert_proto_to_json_str(local) << std::endl;
    std::cout << "error: " << error_value << std::endl;
#endif // DEBUG
    return error_value;
}

