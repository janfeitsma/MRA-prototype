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

// Transformation definition
// These are used to transform from one coordinate system to an other
// We assume there is no z offset.
typedef struct xform
{
  double x; // x offset
  double y; // y offset
  double rz; // rotation in radials
} xform_t;

static xform_t fromto(const MRA::Datatypes::Pose& in0, const MRA::Datatypes::Pose& in1)
{
	xform_t out;
	double r, s, c;

	/*
	    Suppose I have two vectors f0 anf f1 pointing to different
	    locations. Then what is the corresponding transform to get from
	    f0 to f1 ?

		    f1 = xf * f0
		    f1 inv(f0)= xf * f0 * inv(f0)
		    f1 inv(f0)= xf

		from in0 to in1:

		xform = inv(in0) then in1

	    start at 0, then go back in0, then go forward in1
	*/

	r = -in0.rz();
	s = sin(r);
	c = cos(r);

	out.x = -(c * in0.x() - s * in0.y());
	out.y = -(s * in0.x() + c * in0.y());
	out.rz = -in0.rz();

	out.x += (c * in1.x() - s * in1.y());
	out.y += (s * in1.x() + c * in1.y());
	out.rz += in1.rz();
	return out;
}

static MRA::Datatypes::Pose xform_vel(const MRA::Datatypes::Pose& r_in, const xform_t& r_xf)
{
	MRA::Datatypes::Pose out;
	out.set_x(cos(r_xf.rz) * r_in.x() - sin(r_xf.rz) * r_in.y() );
	out.set_y(sin(r_xf.rz) * r_in.x() + cos(r_xf.rz) * r_in.y());
	out.set_z(r_in.z());
	out.set_rz(r_in.rz());
	return out;
}

static double pos_regulator( double observed_pos_error, double vel_throttling, double acc, double delta_t )
// Returns new velocity setpoint in order to reduce position error to 0.
// Scalar values only, if multiple axes require regulation, call once for each axis.
// Choose throttling between 0 and 1, lower when there is more uncertainty. E.g. use 0.9
{
	if( acc == 0 ) return 0;
	double throttled_acc = acc * sqrt( vel_throttling );
	double t_left = sqrt( 2 * fabs( observed_pos_error ) / throttled_acc );
	double vel_setpoint;

	if( delta_t == 0 ) return 0;

	if( t_left > POS_REG_TICKS_FINAL * delta_t ) {
		vel_setpoint = SIGN( observed_pos_error ) * std::max( 0.0, t_left - delta_t ) * throttled_acc;
	}
	else {
		vel_setpoint = observed_pos_error / delta_t * POS_REG_P_GAIN_FACTOR;
	}
	return vel_setpoint;
}


static double calc_robot_movement( double current_velocity, double requested_velocity, double acceleration, double delta_t )
// Calculates how far the robot has moved after one tick for a 2nd order motion profile.
// It also calculates the movement correctly in case of partial ramp-down/ups.
{

	bool does_reach_vel_in_tick = ( fabs( current_velocity - requested_velocity ) <= acceleration * delta_t );


	double movement_in_tick = 0.0;

	if (does_reach_vel_in_tick) {
		// time needed to change velocity
		auto delta_velocity = fabs( current_velocity - requested_velocity );
		auto delta_t_change = delta_velocity / acceleration;
		auto dist_to_reach_requested_velocity = (0.5 * acceleration * delta_t_change * delta_t_change);
		auto dist_at_end_velocity = requested_velocity * (delta_t - delta_t_change);
		if (current_velocity > requested_velocity) {
			// decreasing velocity
			auto dist_during_velocity_decrease = current_velocity * delta_t_change - dist_to_reach_requested_velocity;
			movement_in_tick = dist_during_velocity_decrease + dist_at_end_velocity;
		}
		else {
			// increasing velocity
			auto dist_during_velocity_increase = current_velocity * delta_t_change + dist_to_reach_requested_velocity;
			movement_in_tick = dist_during_velocity_increase + dist_at_end_velocity;
		}
	}
	else {
		// requested velocity not reach in tick
		// movement is: x = v*t + 0.5a*t^2
		movement_in_tick = (current_velocity * delta_t) + (0.5 * acceleration * delta_t * delta_t);
	}

	return movement_in_tick;
}

static double calc_robot_velocity( double current_velocity, double requested_velocity, double acceleration, double delta_t )
// Calculates what the velocity is at the end of the tick for a 2nd order motion profile.
{
	double end_velocity;

	double velocity_delta_tick = acceleration * delta_t;
	bool does_reach_vel_in_tick = ( fabs( current_velocity - requested_velocity ) <= velocity_delta_tick );

	if (does_reach_vel_in_tick) {
		// requested velocity is reached in tick
		end_velocity =  requested_velocity;
	}
	else {

		if (current_velocity > requested_velocity) {
			// velocity is decreasing
			end_velocity = current_velocity - velocity_delta_tick;
		}
		else {
			// velocity is increasing
			end_velocity = current_velocity + velocity_delta_tick;
		}
	}
	return end_velocity;
}

static double rot_mod( double r )
{
	return atan2( sin(r), cos(r) );
}

static double bearing_of_object(MRA::Geometry::Pose robot_pos, MRA::Geometry::Pose obj_pos)
// Returns angle where an object is seen from ourselves. Straight ahead is 0, CCW is positive.
{
	return rot_mod( atan2( obj_pos.y - robot_pos.y, obj_pos.x -robot_pos.x ) - robot_pos.rz - M_PI/2 );
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
    MRA::Geometry::Pose setpoint_pos =  input.setpoint().position();

    MRA::Geometry::Pose deltaPosition = setpoint_pos - robot_pos;
    double distance = deltaPosition.size();

//    double lin_limiting = 0.8; /* getso(tm.skills.movetoball.lin_limiting) >> PARAMS */
    double max_acc_lin = std::min( params.max_acceleration_linear(), std::min(prev_target_acc.x(), prev_target_acc.y()));
    double max_acc_rot = std::min( params.max_acceleration_rotation(), prev_target_acc.rz());

	// Calculate angle of ball relative to robot and correct for ball handler position on robot (along positive Y axis)
	double ball_bearing = bearing_of_object( robot_pos, setpoint_pos ) - params.ball_angle_offset();

	// If the ball is in the area of the ball handler, we can move closer to the ball,
	// otherwise we'll keep larger distance
	double min_distance = 0.0;
	if( fabs( ball_bearing ) < params.angle_pickup() ) {
		min_distance = params.dist_picked_up();
	}
	else {
		min_distance = params.dist_against_robot();
	}

	double vision_delay_distance = hypot(robot_vel.y(),robot_vel.x()) * params.vision_delay();

     auto speed = pos_regulator( std::max(distance - min_distance - vision_delay_distance, 0.0), params.linear_limiting(), max_acc_lin, params.dt() );

     //    // Create a unity vector pointing towards the ball, multiply by speed, and add ball velocity
    MRA::Datatypes::Pose setpoint_fc;

    setpoint_fc.set_x(-sin( ball_bearing + robot_pos.rz() ) * speed);
    setpoint_fc.set_y( cos( ball_bearing + robot_pos.rz() ) * speed);

    bool controlBallLeft  = params.ball_touching_left_ball_handler();
    bool controlBallRight = params.ball_touching_right_ball_handler();
    // These ball statuses now include visual ball check, so we don't need to check that anymore

    if( controlBallLeft && controlBallRight ) {
    	// Have ball, Full stop
    	setpoint_fc.set_x(0.0);
    	setpoint_fc.set_y(0.0);
    	setpoint_fc.set_rz(0.0);
    }
    else if( controlBallLeft && !controlBallRight ) {
    	// If we don't have both ball handlers draw current, rotate a bit
    	//logAlways("movetoball: only holding ball at left\n" );
    	setpoint_fc.set_rz(2.0);
    }
    else if( !controlBallLeft && controlBallRight ) {
    	//logAlways("movetoball: only holding ball at right\n" );
    	setpoint_fc.set_rz(-2.0);
    }
    else {
    	// Calculate rotational speed possible to stop at the wanted angle
    	double vision_delay_rotation = robot_vel.rz() * params.vision_delay();

    	double rot_remaining = rot_mod( ball_bearing - vision_delay_rotation );

    	setpoint_fc.set_rz( SIGN(rot_remaining) * pos_regulator( fabs(rot_remaining), params.rotation_limiting(), max_acc_rot, params.dt() ));
    }


    // Transform setpoint to robot coordinates
    MRA::Datatypes::Pose zero_pos;
    xform_t xf_r2f = fromto( robot_pos, zero_pos );
    MRA::Datatypes::Pose setpoint_rc = xform_vel( setpoint_fc, xf_r2f );

    output.mutable_velocity()->CopyFrom(setpoint_rc);
#ifdef DEBUG
    std::cout << "output: " << convert_proto_to_json_str(output) << std::endl;
    std::cout << "state: " << convert_proto_to_json_str(state) << std::endl;
    std::cout << "local: " << convert_proto_to_json_str(local) << std::endl;
    std::cout << "error: " << error_value << std::endl;
#endif // DEBUG
    return error_value;
}
