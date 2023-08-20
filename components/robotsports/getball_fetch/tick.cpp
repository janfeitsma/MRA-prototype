// this file was produced by MRA-codegen.py from template_tick.cpp
// with the intent of letting user add the implementation here

// generated component header:
#include "RobotsportsGetballFetch.hpp"

// dependent libraries:
#include "geometry.hpp"

using namespace MRA;

// custom includes, if any
#include <cmath>

int RobotsportsGetballFetch::RobotsportsGetballFetch::tick
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
        	MRA::Datatypes::Pose robot_pos = ws.robot().position();
        	MRA::Datatypes::Pose robot_vel = ws.robot().velocity();

        	MRA::Datatypes::Pose target_pos_fc  = ws.ball().position();
        	MRA::Datatypes::Pose target_vel_fc  = ws.ball().velocity();

        	target_pos_fc.set_x(target_pos_fc.x() + target_vel_fc.x() * params.vision_delay());
        	target_pos_fc.set_y(target_pos_fc.y() + target_vel_fc.y() * params.vision_delay());
        	MRA::Geometry::Position target_position = target_pos_fc;
        	target_position.faceAwayFrom(ws.robot().position());

            // write output
            output.mutable_target()->mutable_position()->set_x(target_pos_fc.x());
            output.mutable_target()->mutable_position()->set_y(target_pos_fc.y());
            output.mutable_target()->mutable_position()->set_rz(target_position.rz);
        }
    }
    return error_value;
}



