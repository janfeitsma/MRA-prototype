// this file was produced by MRA-codegen.py from template_tick.cpp
// with the intent of letting user add the implementation here

// generated component header:
#include "FalconsGetballFetch.hpp"

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
        output.set_actionresult(PASSED);
    }
    else
    {
        // initialize output before failure-mode checks
        output.set_actionresult(RUNNING);

        // fail when robot is inactive
        if (!ws.robot().active())
        {
            output.set_actionresult(FAILED);
        }

        // fail when there is no ball
        if (!ws.has_ball())
        {
            output.set_actionresult(FAILED);
        }

        // fail when teammember has the ball
        for (auto const &teammember: ws.teammates())
        {
            if (teammember.hasball())
            {
                output.set_actionresult(FAILED);
            }
        }

        // check if not any failure mode was triggered
        if (output.actionresult() == RUNNING)
        {
            // calculate ball speed
            float vx = ws.ball().velocity().x();
            float vy = ws.ball().velocity().y();
            float ball_speed = sqrt(vx * vx + vy * vy);

            // prepare outputs
            Pose current = ws.robot().position();
            Pose target = ws.robot().position();

            // if speed is low enough, then just drive on top of the ball
            if (ball_speed < params.ballspeedthreshold())
            {
                target.set_x(ws.ball().position().x());
                target.set_y(ws.ball().position().y());
            }
            else // otherwise: try to catch up, by making use of ball velocity vector
            {
                target.set_x(ws.ball().position().x() + ws.ball().velocity().x() * params.ballspeedscaling());
                target.set_y(ws.ball().position().y() + ws.ball().velocity().y() * params.ballspeedscaling());
            }

            // set robot facing angle towards ball
            target.set_rz(atan2(target.y() - current.y(), target.x() - current.x()));

            // TODO: this code could be cleaned up a bit by factoring out some geometry library, Vector2D, operations etc.

            // write output
            output.mutable_target()->mutable_position()->set_x(target.x());
            output.mutable_target()->mutable_position()->set_y(target.y());
            output.mutable_target()->mutable_position()->set_rz(target.rz());
        }
    }
    return error_value;
}

