#include <iostream>
#include <iomanip>
#include <pybind11/pybind11.h>
#include "pybind11_protobuf/native_proto_caster.h"

#include "FalconsLocalizationVision.hpp"

namespace py = ::pybind11;


using namespace MRA::FalconsLocalizationVision;


int pywrap_tick_standalone(
        InputType  const           &input,       // input data, type generated from Input.proto
        ParamsType const           &params,      // configuration parameters, type generated from Params.proto
        StateType                  &state,       // state data, type generated from State.proto
        OutputType                 &output,      // output data, type generated from Output.proto
        LocalType                  &local        // local/diagnostics data, type generated from Local.proto
    )
{
    return FalconsLocalizationVision().tick(input, params, state, output, local);
}

PYBIND11_MODULE(pybind_ext, m) {
    pybind11_protobuf::ImportNativeProtoCasters();
    // TODO: m.doc()?
    m.def("tick",
        [](InputType input, ParamsType params, StateType state, OutputType output, LocalType local)
        {
            return pywrap_tick_standalone(input, params, state, output, local);
        },
        pybind11::arg("input"),
        pybind11::arg("params"),
        pybind11::arg("state"),
        pybind11::arg("output"),
        pybind11::arg("local")
    );
}

