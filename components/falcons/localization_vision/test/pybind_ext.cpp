#include <iostream>
#include <iomanip>
#include <pybind11/pybind11.h>
#include "pybind11_protobuf/native_proto_caster.h"

#include "FalconsLocalizationVision.hpp"


using namespace MRA::FalconsLocalizationVision;


int pywrap_tick_standalone(
        InputType  const           &input,       // input data, type generated from Input.proto
        ParamsType const           &params,      // configuration parameters, type generated from Params.proto
        StateType                  &state,       // state data, type generated from State.proto
        OutputType                 &output,      // output data, type generated from Output.proto
        LocalType                  &local        // local/diagnostics data, type generated from Local.proto
    )
{
    int result = FalconsLocalizationVision().tick(input, params, state, output, local);
    return result;
}

PYBIND11_MODULE(pybind_ext, m) {
    pybind11_protobuf::ImportNativeProtoCasters();
    // TODO: m.doc()?
    m.def("tick",
        [](InputType input, ParamsType params, StateType state)
        {
            // input and params are pure inputs
            // state is an inout parameter
            // output and local are pure outputs
            OutputType out;
            LocalType loc;
            int error_code = pywrap_tick_standalone(input, params, state, out, loc);
            pybind11::tuple result_tuple = pybind11::tuple(4);
            result_tuple[0] = error_code;
            result_tuple[1] = state;
            result_tuple[2] = out;
            result_tuple[3] = loc;
            return result_tuple;
        },
        //pybind11::call_guard<pybind11::gil_scoped_release>(),
        pybind11::arg("input"),
        pybind11::arg("params"),
        pybind11::arg("state"), pybind11::return_value_policy::reference//,
        //pybind11::arg("output"),// pybind11::return_value_policy::copy,
        //pybind11::arg("local")//, pybind11::return_value_policy::copy
    );
}

