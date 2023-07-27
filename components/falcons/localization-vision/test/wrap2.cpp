#include <pybind11/pybind11.h>
#include "wrap2.hpp"

using namespace MRA::FalconsLocalizationVision;

PYBIND11_MODULE(pybind11_example_module, m) {
    m.doc() = "pybind11 example plugin"; // Optional module docstring
    m.def("cpp_function", &pywrap_tick_standalone, "test function");
}

int pywrap_tick_standalone(
        google::protobuf::Timestamp timestamp,   // absolute timestamp
        InputType  const           &input,       // input data, type generated from Input.proto
        ParamsType const           &params,      // configuration parameters, type generated from Params.proto
        StateType                  &state,       // state data, type generated from State.proto
        OutputType                 &output,      // output data, type generated from Output.proto
        LocalType                  &local        // local/diagnostics data, type generated from Local.proto
    )
{
    return FalconsLocalizationVision().tick(timestamp, input, params, state, output, local);
}

