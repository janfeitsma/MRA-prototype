// this file was produced by MRA-codegen.py from template_tick.cpp
// with the intent of letting user add the implementation here

// generated component header:
#include "FalconsGetballFetch.hpp"

// dependent libraries:
#include "geometry.hpp"

using namespace MRA;

// custom includes, if any
#include <cmath>


int FalconsGetballFetch::FalconsGetballFetch::tick
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

    auto const ws = input.worldstate();

    if (ws.robot().hasball())
    {
        // the only success is robot having the ball
        output.set_actionresult(MRA::Datatypes::PASSED);
    }
    else
    {
        // initialize output before failure-mode checks
        output.set_actionresult(MRA::Datatypes::RUNNING);

        // fail when robot is inactive
        if (!ws.robot().active())
        {
            output.set_actionresult(MRA::Datatypes::FAILED);
        }

        // fail when there is no ball
        if (!ws.has_ball())
        {
            output.set_actionresult(MRA::Datatypes::FAILED);
        }

        // fail when teammember has the ball
        for (auto const &teammember: ws.teammates())
        {
            if (teammember.hasball())
            {
                output.set_actionresult(MRA::Datatypes::FAILED);
            }
        }

        // check if not any failure mode was triggered
        if (output.actionresult() == MRA::Datatypes::RUNNING)
        {
            MRA::Geometry::Position ball_position(ws.ball().position());
            MRA::Geometry::Velocity ball_velocity(ws.ball().velocity());

            // if speed is low enough, then just drive on top of the ball
            // otherwise: try to catch up, by making use of ball velocity vector
            float ball_speed = ball_velocity.size();
            float factor = params.ballspeedscaling() * (ball_speed >= params.ballspeedthreshold());

            // set target, robot facing angle towards ball
            MRA::Geometry::Position target = ball_position + ball_velocity * factor;
            target.faceAwayFrom(ws.robot().position());

            // write output
            output.mutable_target()->mutable_position()->set_x(target.x);
            output.mutable_target()->mutable_position()->set_y(target.y);
            output.mutable_target()->mutable_position()->set_rz(target.rz);
        }
    }
    return error_value;
}

