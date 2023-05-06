/*
 * ShiftBallOffset.cpp
 *
 *  Created on: December, 2019
 *      Author: Jan Feitsma
 */

#include "VelocityControlAlgorithms.hpp"


// to enable dribbling, limits should apply to ball, not robot, if so configured
// especially backward driving and X strafing needs to be carefully tuned, to prevent losing the ball
// (if limits would be applied to robot, then a rotation setpoint can cause a large X jolt at the ball)

void ShiftBallOffset::execute(VelocityControlData &data)
{
    // add ball offset, if applicable
    if (data.input.worldstate().has_ball() && data.config.dribble().applylimitstoball())
    {
        auto offset = MRA::Geometry::Pose(0.0, data.config.dribble().radiusrobottoball(), 0.0);
        data.targetPositionFcs.addRcsToFcs(offset);
        data.currentPositionFcs.addRcsToFcs(offset);
        data.previousPositionSetpointFcs.addRcsToFcs(offset); // for SPG
    }
}

void UnShiftBallOffset::execute(VelocityControlData &data)
{
    // subtract ball offset, if applicable
    if (data.input.worldstate().has_ball() && data.config.dribble().applylimitstoball())
    {
        // resultVelocityRcs applies to the ball
        // convert to motor setpoint
        // this used to be called TokyoDrift
        data.resultVelocityRcs.x += data.resultVelocityRcs.rz * data.config.dribble().radiusrobottoball();

        // unshift coordinates
        // - SPG 'feedforward' data update
        auto offset = MRA::Geometry::Pose(0.0, -data.config.dribble().radiusrobottoball(), 0.0);
        data.targetPositionFcs.addRcsToFcs(offset);
        data.currentPositionFcs.addRcsToFcs(offset);
        data.previousPositionSetpointFcs.addRcsToFcs(offset); // for SPG
    }
}

