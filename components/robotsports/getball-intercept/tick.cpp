// this file was produced by MRA-codegen.py from template_tick.cpp
// with the intent of letting user add the implementation here

// generated component header:
#include "RobotsportsGetballIntercept.hpp"

// dependent libraries:
#include "geometry.hpp"

using namespace MRA;

// custom includes, if any
// ...

/**
 * Calculates the intersection point of line given by pose1  and pose2, and the line perpendicular to (pose1,pose2) going
 * through point pose_p.
 *
 * @param px
 *            X of Point though which the perpendicular line must go
 * @param py
 *            Y of Point though which the perpendicular line must go
 * @param p1x
 *            X of One point on the line (p1,p2)
 * @param p1y
 *            Y of One point on the line (p1,p2)
 * @param p2x
 *            X of Other point on the line (p1,p2)
 * @param p2y
 *            Y ofOther point on the line (p1,p2)
 * @return Intersection point D. Note that c.distanceTo(D) is the distance of c to the line (p1,p2).
 */
static MRA::Geometry::Position intersectPerpendicular(const MRA::Geometry::Position& r_pose1, const MRA::Geometry::Position& r_pose2,
	const MRA::Geometry::Position& r_pose_p) {
	double p1x = r_pose1.x;
	double p1y = r_pose1.y;
	double p2x = r_pose2.x;
	double p2y = r_pose2.y;
	double px = r_pose_p.x;
	double py = r_pose_p.y;
	double lambda =((p2x - p1x) * (px - p1x) + (p2y - p1y) * (py - p1y)) / ((p2x - p1x) * (p2x - p1x) + (p2y - p1y) * (p2y - p1y));
	MRA::Geometry::Position intersection;
	intersection.x = p1x + lambda * (p2x - p1x);
	intersection.y = p1y + lambda * (p2y - p1y);
	return intersection;
}

static double distanceXY(const MRA::Geometry::Position& r_pose1, const MRA::Geometry::Position& r_pose2) {
	return hypot(r_pose1.x - r_pose2.x, r_pose1.y - r_pose2.y);
}


int RobotsportsGetballIntercept::RobotsportsGetballIntercept::tick
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
    std::cerr << __FILE__ << " timestamp: " << timestamp << std::endl;
    std::cout << __FILE__ << " input: " << convert_proto_to_json_str(input) << std::endl;
    std::cout << __FILE__ << " params: " << convert_proto_to_json_str(params) << std::endl;
    std::cout << __FILE__ << " state: " << convert_proto_to_json_str(state) << std::endl;
#endif // DEBUG

	int error_value = 0;

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

        // check if not any failure mode was triggered
        if (output.actionresult() == MRA::Datatypes::RUNNING)
        {
        	MRA::Datatypes::Pose robot_pos = ws.robot().position();
        	MRA::Datatypes::Pose robot_vel = ws.robot().velocity();

        	MRA::Datatypes::Pose ball_pos_fc  = ws.ball().position();
        	MRA::Datatypes::Pose target_vel_fc  = ws.ball().velocity();

        	// get extrapolated ball position : add velocity vector (position over 1 sec)
        	MRA::Geometry::Position ball_pos_fc_extrapolated = ball_pos_fc;
        	ball_pos_fc_extrapolated += target_vel_fc;

        	MRA::Geometry::Position target_position = intersectPerpendicular(ball_pos_fc, ball_pos_fc_extrapolated, robot_pos);
        	target_position.faceTowards(ball_pos_fc); // facing ball


            if (distanceXY(target_position, robot_pos) > params.actionradius())
            {
                // To far from interception point:
                // - Mark as failed. 
                // - Stay at current position (safe output values). 
                output.set_actionresult(MRA::Datatypes::FAILED);
                output.mutable_target()->mutable_position()->set_x(robot_pos.x());
                output.mutable_target()->mutable_position()->set_y(robot_pos.y());
                output.mutable_target()->mutable_position()->set_rz(robot_pos.rz());
            }
            else {
                // write output
                output.mutable_target()->mutable_position()->set_x(target_position.x);
                output.mutable_target()->mutable_position()->set_y(target_position.y);
                output.mutable_target()->mutable_position()->set_rz(target_position.rz);
            }
        }
    }
#ifdef DEBUG
    std::cout << __FILE__ << " output: " << convert_proto_to_json_str(output) << std::endl;
    std::cerr << __FILE__ << " output: " << convert_proto_to_json_str(output) << std::endl;
    std::cout << __FILE__ << " state: " << convert_proto_to_json_str(state) << std::endl;
    std::cout << __FILE__ << " local: " << convert_proto_to_json_str(local) << std::endl;
    std::cout << __FILE__ << " error: " << error_value << std::endl;
#endif // DEBUG
    return error_value;
}

