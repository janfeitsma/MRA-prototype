// this file was produced by MRA-codegen.py from template_tick.cpp
// with the intent of letting user add the implementation here

// generated component header:
#include "FalconsLocalizationVision.hpp"

using namespace MRA;

// custom includes, if any
#include "determinePosition.hpp"

int FalconsLocalizationVision::FalconsLocalizationVision::tick
(
    double            timestamp,   // simulation timestamp, seconds since start of simulation
    InputType  const &input,       // input data, type generated from Input.proto
    ParamsType const &params,      // configuration parameters, type generated from Params.proto
    StateType        &state,       // state data, type generated from State.proto
    OutputType       &output,      // output data, type generated from Output.proto
    LocalType        &local        // local/diagnostics data, type generated from Local.proto
)
{
    int error_value = 0;

    // user implementation goes here

    // setup
    // TODO: how expensive is it to reconstruct everything each tick?
    configurator conf = configurator(0);
    linePointDetection linePoint;
    robotFloor rFloor(&conf);
    determinePosition detPos(&conf, &linePoint, &rFloor);

    // call
    detPos.pointsToPosition();
    std::vector<detPosSt> locResult = detPos.getLocList();

    return error_value;
}

