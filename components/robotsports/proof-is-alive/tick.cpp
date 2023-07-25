// this file was produced by MRA-codegen.py from template_tick.cpp
// with the intent of letting user add the implementation here

// generated component header:
#include "RobotsportsProofIsAlive.hpp"

#include "logging.hpp" // TODO: automate, perhaps via generated hpp

using namespace MRA;

// custom includes, if any
// #define DEBUG

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
	return angle_in_degrees * (M_PI / 180.0);
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
#ifdef DEBUG
    std::cout << "timestamp: " << timestamp << std::endl;
    std::cout << "input: " << convert_proto_to_json_str(input) << std::endl;
    std::cout << "params: " << convert_proto_to_json_str(params) << std::endl;
    std::cout << "state: " << convert_proto_to_json_str(state) << std::endl;
#endif // DEBUG

	int error_value = 0;
	MRA::Logging::LogTick scoped(timestamp, input, params, &state, &output, &local, &error_value);

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
	    std::cout << "TIMEOUT: FAILED due to too much time between phases (max: " << std::setprecision(2) << max_time_per_phase << " seconds)" << std::endl;
#endif // DEBUG
        output.set_actionresult(MRA::Datatypes::FAILED);
	}
	else if (!ws.robot().active())
    {
		// fail when robot is inactive
		output.set_actionresult(MRA::Datatypes::FAILED);
    }
	else
	{

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
		    	state.mutable_requested_position()->set_rz(ws.robot().position().rz() - rotation_angle_rad);
				state.set_phase(StateType::FINISHED);
	        	output.set_actionresult(MRA::Datatypes::PASSED);
			}
		}
	    output.mutable_target()->mutable_position()->set_x(state.requested_position().x());
	    output.mutable_target()->mutable_position()->set_y(state.requested_position().y());
	    output.mutable_target()->mutable_position()->set_rz(state.requested_position().rz());
	}

//#ifdef DEBUG
//    std::cout << "output: " << convert_proto_to_json_str(output) << std::endl;
//    std::cout << "state: " << convert_proto_to_json_str(state) << std::endl;
//    std::cout << "local: " << convert_proto_to_json_str(local) << std::endl;
//    std::cout << "error: " << error_value << std::endl;
//#endif // DEBUG

    return error_value;
}

