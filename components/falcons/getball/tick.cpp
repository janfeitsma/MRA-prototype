// this file was produced by MRA-codegen.py from template_tick.cpp
// with the intent of letting user add the implementation here

// generated component header:
#include "FalconsGetball.hpp"

// dependent (generated) component headers:
#include "FalconsGetballFetch.hpp"
#include "FalconsGetballIntercept.hpp"

using namespace MRA;

// custom includes, if any
#include <cmath>


int FalconsGetball::FalconsGetball::tick
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

    float vx = input.worldstate().ball().velocity().x();
    float vy = input.worldstate().ball().velocity().y();
    float ball_speed = sqrt(vx * vx + vy * vy);

    if (ball_speed < params.ballspeedstationarythreshold())
    {
        // call component: FalconsGetballFetch
        FalconsGetballFetch::InputType subcomponent_input;
        FalconsGetballFetch::OutputType subcomponent_output;
        //fbi.worldstate() = input.worldstate();
        error_value = FalconsGetballFetch::FalconsGetballFetch().tick(
            timestamp,
            subcomponent_input,
            params.fetch(),
            state,
            subcomponent_output,
            local
        );
        //output.set_actionresult(subcomponent_output.actionresult());
        //output.set_target(subcomponent_output.target());
    }
/*    else
    {
        // TODO: determine direction relative to robot
        error_value = FalconsGetballIntercept().tick(
            timestamp,
            input,
            params.fetch,
            state,
            output,
            local
        );
    }
*/

    return error_value;
}

