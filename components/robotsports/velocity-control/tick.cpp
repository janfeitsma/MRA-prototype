// this file was produced by MRA-codegen.py from template_tick.cpp
// with the intent of letting user add the implementation here

// generated component header:
#include "RobotsportsVelocityControl.hpp"

using namespace MRA;

//#define DEBUG

// internals


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

    // user implementation goes here
    //
    //Pose_to_pos_t(target_pos, output.target().position());
    //double distance = distance2D(me_pos_field, target_pos);
    //double lin_limiting = 0.8; /* getso(tm.skills.movetoball.lin_limiting) */
    //
    //double max_acc_lin = MIN( trs::DriveInterface::get_max_acceleration_linear(), MIN( trs::DriveInterface::get_target_acceleration().x, trs::DriveInterface::get_target_acceleration().y)); // TODO: use separate x and y acceleration values
    //double max_acc_rot = MIN( trs::DriveInterface::get_max_acceleration_rotation(), trs::DriveInterface::get_target_acceleration().r);
    //double dt = getso( min_tick_duration );
    //
    //auto speed = pos_regulator( distance, lin_limiting, max_acc_lin, dt );
    //pos_t setpoint_fc = {};
    //
    //// Create a unity vector pointing towards the ball, multiply by speed, and add ball velocity
    //double ball_bearing = bearing_of_object( me_pos_field, target_pos ) - getso(tm.skills.movetoball.ball_angle_offset);
    //setpoint_fc.x = -sin( ball_bearing + me_pos_field.r ) * speed;
    //setpoint_fc.y =  cos( ball_bearing + me_pos_field.r ) * speed;
    //pos_t zero_pos  = { 0.0, 0.0, 0.0, 0.0 };
    //
    //bool controlBallLeft  = trs::WmInterface::ball_touching_left_ball_handler();
    //bool controlBallRight = trs::WmInterface::ball_touching_right_ball_handler();
    //// These ball statuses now include visual ball check, so we don't need to check that anymore
    //
    //if( controlBallLeft && controlBallRight ) {
    //	// Have ball, Full stop
    //	setpoint_fc.x = 0;
    //	setpoint_fc.y = 0;
    //	setpoint_fc.r = 0;
    //}
    //else if( controlBallLeft && !controlBallRight ) {
    //	// If we don't have both ball handlers draw current, rotate a bit
    //	//logAlways("movetoball: only holding ball at left\n" );
    //	setpoint_fc.r = 2;
    //}
    //else if( !controlBallLeft && controlBallRight ) {
    //	//logAlways("movetoball: only holding ball at right\n" );
    //	setpoint_fc.r = -2;
    //}
    //else {
    //	// Calculate rotational speed possible to stop at the wanted angle
    //	double vision_delay = 0.10; // getso( tm.skills.movetoball.vision_delay );
    //	double rot_limiting = 0.8; // getso( tm.skills.movetoball.rot_limiting );
    //	double vision_delay_rotation = me_speed.r * vision_delay;
    //
    //	double rot_remaining = rot_mod( ball_bearing - vision_delay_rotation );
    //
    //	setpoint_fc.r = SIGN(rot_remaining) * pos_regulator( fabs(rot_remaining), rot_limiting, max_acc_rot, dt );
    //}
    //
    //
    // Transform setpoint to robot coordinates
    //xform_t xf_r2f;
    //fromto( &xf_r2f, me_pos_field, zero_pos );
    //pos_t setpoint_rc = {};
    //xform_vel( &setpoint_rc, &setpoint_fc, &xf_r2f );
    //
    //// Write shared memory variables for the motion system
    //trs::DriveInterface::set_target_velocity(setpoint_rc);
    //logAlways("getball_falcons: setpoint x: %4.2f y: %4.2f r: %4.2f z: %4.2f", setpoint_rc.x, setpoint_rc.y, setpoint_rc.r, setpoint_rc.z);
    //logAlways("getball_falcons: THE END -> %d", __LINE__);
    //}

#ifdef DEBUG
    std::cout << "output: " << convert_proto_to_json_str(output) << std::endl;
    std::cout << "state: " << convert_proto_to_json_str(state) << std::endl;
    std::cout << "local: " << convert_proto_to_json_str(local) << std::endl;
    std::cout << "error: " << error_value << std::endl;
#endif // DEBUG
    return error_value;
}

