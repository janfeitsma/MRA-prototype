// this file was produced by MRA-codegen.py from template_tick.cpp
// with the intent of letting user add the implementation here

// generated component header:
#include "RobotsportsVelocityControl.hpp"

#include <algorithm>

// dependent libraries:
#include "geometry.hpp"


using namespace MRA;

#define DEBUG

// internals



#define	POS_REG_TICKS_FINAL 2 // If fewer than this amount of ticks is left to reach end position, switch to proportional regulation
#define SIGN(a) (((a)>0)-((a)<0))
#define POS_REG_P_GAIN_FACTOR (1/POS_REG_TICKS_FINAL)

//static double pos_regulator( double observed_pos_error, double vel_throttling, double acc, double delta_t )
//// Returns new velocity setpoint in order to reduce position error to 0.
//// Scalar values only, if multiple axes require regulation, call once for each axis.
//// Choose throttling between 0 and 1, lower when there is more uncertainty. E.g. use 0.9
//{
//	if( acc == 0 ) return 0;
//	double throttled_acc = acc * sqrt( vel_throttling );
//	double t_left = sqrt( 2 * fabs( observed_pos_error ) / throttled_acc );
//	double vel_setpoint;
//
//	if( delta_t == 0 ) return 0;
//
//	if( t_left > POS_REG_TICKS_FINAL * delta_t ) {
//		vel_setpoint = SIGN( observed_pos_error ) * std::max( 0.0, t_left - delta_t ) * throttled_acc;
//	}
//	else {
//		vel_setpoint = observed_pos_error / delta_t * POS_REG_P_GAIN_FACTOR;
//	}
//	return vel_setpoint;
//}


static double calc_robot_velocity( double current_velocity, double required_velocity, double acceleration, double delta_time )
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

static double calc_robot_movement( double cur_vel, double req_vel, double acc, double delta_t )
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


int RobotsportsVelocityControl::RobotsportsVelocityControl::tick
(
    double            timestamp,   // simulation timestamp, seconds since start of simulation
    InputType  const &input,       // input data, type generated from Input.proto
    ParamsType const &params,      // configuration parameters, type generated from Params.proto
    StateType        &state,       // state data, type generated from State.proto
    OutputType       &output,      // output data, type generated from Output.proto
    LocalType        &local        // local/diagnostics data, type generated from Local.proto
)
{
#ifdef DEBUG
    std::cout << "timestamp: " << timestamp << std::endl;
    std::cout << "input: " << convert_proto_to_json_str(input) << std::endl;
    std::cout << "params: " << convert_proto_to_json_str(params) << std::endl;
    std::cout << "state: " << convert_proto_to_json_str(state) << std::endl;
#endif // DEBUG
    int error_value = 0;

    MRA::Datatypes::Pose prev_target_vel = params.requested_velocity();
    MRA::Datatypes::Pose prev_target_acc  = params.requested_acceleration();

    MRA::Datatypes::Pose next_pos;
    MRA::Datatypes::Pose next_vel;

    auto const ws = input.worldstate();
    MRA::Datatypes::Pose robot_pos = ws.robot().position();
	MRA::Datatypes::Pose robot_vel = ws.robot().velocity();

    next_pos.set_x(robot_pos.x() + calc_robot_movement( robot_vel.x(), prev_target_vel.x(), prev_target_acc.x(), params.dt() ));
    next_pos.set_y(robot_pos.y() + calc_robot_movement( robot_vel.y(), prev_target_vel.y(), prev_target_acc.y(), params.dt() ));
    next_pos.set_rz(robot_pos.rz() + calc_robot_movement( robot_vel.rz(), prev_target_vel.rz(), prev_target_acc.rz(), params.dt() ));

    next_vel.set_x( calc_robot_velocity( robot_vel.x(), prev_target_vel.x(), prev_target_acc.x(), params.dt() ));
    next_vel.set_y( calc_robot_velocity( robot_vel.y(), prev_target_vel.y(), prev_target_acc.y(), params.dt() ));
    next_vel.set_rz( calc_robot_velocity( robot_vel.rz(), prev_target_vel.rz(), prev_target_acc.rz(), params.dt() ));

//    // user implementation goes here
//    //
//    //Pose_to_pos_t(target_pos, output.target().position());
//    MRA::Geometry::Pose setpoint_pos =  input.setpoint().position();
//
//    MRA::Geometry::Pose deltaPosition = setpoint_pos - robot_pos;
//    double distance = deltaPosition.size();
//    double lin_limiting = 0.8; /* getso(tm.skills.movetoball.lin_limiting) >> PARAMS */
//    double max_acc_lin = std::min( params.max_acceleration_linear, std::min( trs::DriveInterface::get_target_acceleration().x, trs::DriveInterface::get_target_acceleration().y));
//    double max_acc_rot = std::min( params.max_acceleration_rotation, trs::DriveInterface::get_target_acceleration().r);
//    double dt = 0.025; // TODO getso( min_tick_duration );
//    //
//    auto speed = pos_regulator( distance, lin_limiting, max_acc_lin, dt );
//    pos_t setpoint_fc = {};
//
//    // Create a unity vector pointing towards the ball, multiply by speed, and add ball velocity
//    double ball_bearing = bearing_of_object( me_pos_field, target_pos ) - getso(tm.skills.movetoball.ball_angle_offset);
//    setpoint_fc.x = -sin( ball_bearing + me_pos_field.r ) * speed;
//    setpoint_fc.y =  cos( ball_bearing + me_pos_field.r ) * speed;
//    pos_t zero_pos  = { 0.0, 0.0, 0.0, 0.0 };
//
//    bool controlBallLeft  = trs::WmInterface::ball_touching_left_ball_handler();
//    bool controlBallRight = trs::WmInterface::ball_touching_right_ball_handler();
//    // These ball statuses now include visual ball check, so we don't need to check that anymore
//
//    if( controlBallLeft && controlBallRight ) {
//    	// Have ball, Full stop
//    	setpoint_fc.x = 0;
//    	setpoint_fc.y = 0;
//    	setpoint_fc.r = 0;
//    }
//    else if( controlBallLeft && !controlBallRight ) {
//    	// If we don't have both ball handlers draw current, rotate a bit
//    	//logAlways("movetoball: only holding ball at left\n" );
//    	setpoint_fc.r = 2;
//    }
//    else if( !controlBallLeft && controlBallRight ) {
//    	//logAlways("movetoball: only holding ball at right\n" );
//    	setpoint_fc.r = -2;
//    }
//    else {
//    	// Calculate rotational speed possible to stop at the wanted angle
//    	double vision_delay = 0.10; // getso( tm.skills.movetoball.vision_delay );
//    	double rot_limiting = 0.8; // getso( tm.skills.movetoball.rot_limiting );
//    	double vision_delay_rotation = me_speed.r * vision_delay;
//
//    	double rot_remaining = rot_mod( ball_bearing - vision_delay_rotation );
//
//    	setpoint_fc.r = SIGN(rot_remaining) * pos_regulator( fabs(rot_remaining), rot_limiting, max_acc_rot, dt );
//    }
//
//
//     Transform setpoint to robot coordinates
//    xform_t xf_r2f;
//    fromto( &xf_r2f, me_pos_field, zero_pos );
//    pos_t setpoint_rc = {};
//    xform_vel( &setpoint_rc, &setpoint_fc, &xf_r2f );
//
//    // Write shared memory variables for the motion system
//    trs::DriveInterface::set_target_velocity(setpoint_rc);
//    logAlways("getball_falcons: setpoint x: %4.2f y: %4.2f r: %4.2f z: %4.2f", setpoint_rc.x, setpoint_rc.y, setpoint_rc.r, setpoint_rc.z);
//    logAlways("getball_falcons: THE END -> %d", __LINE__);
//    }

#ifdef DEBUG
    std::cout << "output: " << convert_proto_to_json_str(output) << std::endl;
    std::cout << "state: " << convert_proto_to_json_str(state) << std::endl;
    std::cout << "local: " << convert_proto_to_json_str(local) << std::endl;
    std::cout << "error: " << error_value << std::endl;
#endif // DEBUG
    return error_value;
}
