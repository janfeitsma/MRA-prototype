/*
 * ShiftBallOffset.cpp
 *
 *  Created on: December, 2019
 *      Author: Jan Feitsma
 */

#include "VelocityControlAlgorithms.hpp"
#include "facilities/vcgeometry.hpp"


void ShiftBallOffset::execute(VelocityControlData &data)
{
    TRACE_FUNCTION("");

    // add ball offset, if applicable
    if (data.robot.hasBall && data.ppConfig.forwardDriving.applyLimitsToBall)
    {
        auto offset = Position2D(0.0, data.ppConfig.forwardDriving.radiusRobotToBall, 0.0);
        data.targetPositionFcs = addRcsToFcs(offset, data.targetPositionFcs);
        data.currentPositionFcs = addRcsToFcs(offset, data.currentPositionFcs);

        // for SPG
        data.previousPositionSetpointFcs = addRcsToFcs(offset, data.previousPositionSetpointFcs);
    }
}

