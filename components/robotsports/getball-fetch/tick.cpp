// this file was produced by MRA-codegen.py from template_tick.cpp
// with the intent of letting user add the implementation here

// generated component header:
#include "RobotsportsGetballFetch.hpp"

// dependent libraries:
#include "geometry.hpp"

using namespace MRA;

// custom includes, if any
#include <cmath>

static double calc_robot_movement( double cur_vel, double req_vel, double acc, double delta_t );
static double calc_robot_velocity( double current_velocity, double required_velocity, double acceleration, double delta_time );

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

        	MRA::Datatypes::Pose prev_target_vel = params.requested_velocity();
        	MRA::Datatypes::Pose prev_target_acc  = params.requested_acceleration();

        	MRA::Datatypes::Pose next_pos;
        	MRA::Datatypes::Pose next_vel;

        	next_pos.set_x(robot_pos.x() + calc_robot_movement( robot_vel.x(), prev_target_vel.x(), prev_target_acc.x(), params.tick_duration() ));
        	next_pos.set_y(robot_pos.y() + calc_robot_movement( robot_vel.y(), prev_target_vel.y(), prev_target_acc.y(), params.tick_duration() ));
        	next_pos.set_rz(robot_pos.rz() + calc_robot_movement( robot_vel.rz(), prev_target_vel.rz(), prev_target_acc.rz(), params.tick_duration() ));
        	next_vel.set_x( calc_robot_velocity( robot_vel.x(), prev_target_vel.x(), prev_target_acc.x(), params.tick_duration() ));
        	next_vel.set_y( calc_robot_velocity( robot_vel.y(), prev_target_vel.y(), prev_target_acc.y(), params.tick_duration() ));
        	next_vel.set_rz( calc_robot_velocity( robot_vel.rz(), prev_target_vel.rz(), prev_target_acc.rz(), params.tick_duration() ));

        	MRA::Datatypes::Pose target_pos_fc  = ws.ball().position();
        	MRA::Datatypes::Pose target_vel_fc  = ws.ball().velocity();

        	target_pos_fc.set_x(target_pos_fc.x() + target_vel_fc.x() * params.vision_delay());
        	target_pos_fc.set_y(target_pos_fc.y() + target_vel_fc.y() * params.vision_delay());
			target_pos_fc.set_rz(target_pos_fc.rz() + target_vel_fc.rz() * params.vision_delay());

            // write output
            output.mutable_target()->mutable_position()->set_x(target_pos_fc.x());
            output.mutable_target()->mutable_position()->set_y(target_pos_fc.y());
            output.mutable_target()->mutable_position()->set_rz(target_pos_fc.rz());
        }
    }
    return error_value;
}

double calc_robot_movement( double cur_vel, double req_vel, double acc, double delta_t )
// Calculates how far the robot has moved after one tick.
// It also calculates the movement correctly in case of partial ramp-down/ups.
{
	bool will_reach_vel = ( fabs( cur_vel - req_vel ) <= acc * delta_t );

	double delta_t_vel_reached = will_reach_vel ? fabs( cur_vel - req_vel ) / acc : delta_t;
	double delta_t_vel_ramping = delta_t - delta_t_vel_reached;

	double end_vel = calc_robot_velocity(cur_vel, req_vel, acc, delta_t );

	double movement = (cur_vel + end_vel) / 2 * delta_t_vel_ramping + end_vel * delta_t_vel_reached;

	return movement;
}

double calc_robot_velocity( double current_velocity, double required_velocity, double acceleration, double delta_time )
// Calculates what the velocity is at the end of the tick.
{
	double robot_velocity_end_of_tick;
	double delta_acceleration = acceleration * delta_time;
	if (required_velocity < current_velocity + delta_acceleration) {
		robot_velocity_end_of_tick = current_velocity + delta_acceleration;
	}
	else {
		if (required_velocity > current_velocity - delta_acceleration) {
			robot_velocity_end_of_tick = current_velocity - delta_acceleration;
		}
		else {
			robot_velocity_end_of_tick = required_velocity;
		}
	}
	return robot_velocity_end_of_tick;
}


