// this file was produced by MRA-codegen.py from template_tick.cpp
// with the intent of letting user add the implementation here

// generated component header:
#include "RobotsportsProofIsAlive.hpp"

using namespace MRA;

// custom includes, if any


// -----------------------------------------------------------------------------
// get shortest angle between two angle [rad]
static double min_angle(double start_angle, double end_angle)
{
	double x1 = cos(start_angle);
	double y1 = sin(start_angle);
	double x2 = cos(end_angle);
	double y2 = sin(end_angle);

	double dot_product = x1*x2 + y1*y2;
	return acos(dot_product);
}

// -----------------------------------------------------------------------------
// convert angle given in degrees to radians
static double deg2rad(double angle_in_degrees)
{
	return angle_in_degrees * M_PI * 180.0;
}

// -----------------------------------------------------------------------------
int RobotsportsProofIsAlive::RobotsportsProofIsAlive::tick
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
    auto const ws = input.worldstate();
	double rotation_angle_rad = deg2rad(params.angle_in_degrees());

	if (state.phase() == StateType::TO_BE_STARTED) 
	{
	    state.set_timestamp_start_phase(timestamp);		
	}

	double max_time_per_phase = params.max_time_per_phase();
	if (timestamp - state.timestamp_start_phase() > max_time_per_phase)
	{
		// time out: to long before reaching new state
#ifdef DEBUG
	    std::cout << "TIMEOUT -> deactived - too much time between phases (max: " << std::setprecision(2) << max_time_per_phase << " seconds)" << std::endl;
#endif // DEBUG
        output.set_actionresult(MRA::Datatypes::FAILED);
	}

    output.set_actionresult(MRA::Datatypes::RUNNING);

	if (state.phase() == StateType::TO_BE_STARTED) 
	{
	    state.set_timestamp_start_phase(timestamp);		
	    state.mutable_requested_position()->set_x(ws.robot().position().x());
	    state.mutable_requested_position()->set_y(ws.robot().position().y());

	    state.mutable_requested_position()->set_rz(ws.robot().position().rz() + rotation_angle_rad);
		state.set_phase(StateType::TURN_TO_LEFT);
	}
	else if (state.phase() == StateType::TURN_TO_LEFT) 
	{
		if (min_angle(state.requested_position().rz(), ws.robot().position().rz()) < deg2rad(params.angle_tolerance_deg()))
		{
			// Robot is turned to the left
		    state.set_timestamp_start_phase(timestamp);		
	    	state.mutable_requested_position()->set_x(ws.robot().position().x());
	    	state.mutable_requested_position()->set_y(ws.robot().position().y());
	    	state.mutable_requested_position()->set_rz(ws.robot().position().rz() - 2 * rotation_angle_rad);
			state.set_phase(StateType::TURN_TO_RIGHT);
		}
	}
	else if (state.phase() == StateType::TURN_TO_RIGHT) 
	{
		if (min_angle(state.requested_position().rz(), ws.robot().position().rz()) < deg2rad(params.angle_tolerance_deg()))
		{
		    state.set_timestamp_start_phase(timestamp);		
	    	state.mutable_requested_position()->set_x(ws.robot().position().x());
	    	state.mutable_requested_position()->set_y(ws.robot().position().y());
	    	state.mutable_requested_position()->set_rz(ws.robot().position().rz() + rotation_angle_rad);
			state.set_phase(StateType::TURN_TO_MIDDLE);
		}
	}
	else if (state.phase() == StateType::TURN_TO_MIDDLE) 
	{
		if (min_angle(state.requested_position().rz(), ws.robot().position().rz()) < deg2rad(params.angle_tolerance_deg()))
		{
		    state.set_timestamp_start_phase(timestamp);		
	    	state.mutable_requested_position()->set_x(ws.robot().position().x());
	    	state.mutable_requested_position()->set_y(ws.robot().position().y());
	    	state.mutable_requested_position()->set_rz(ws.robot().position().rz() + rotation_angle_rad);
			state.set_phase(StateType::FINISHED);
        	output.set_actionresult(MRA::Datatypes::PASSED);
		}
	}

    return error_value;
}

