/*
 * UnShiftBallOffset.cpp
 *
 *  Created on: December, 2019
 *      Author: Jan Feitsma
 */

#include "VelocityControlAlgorithms.hpp"
#include "facilities/vcgeometry.hpp"


void UnShiftBallOffset::execute(VelocityControlData &data)
{

    if (data.robot.hasBall && data.ppConfig.forwardDriving.applyLimitsToBall)
    {
        // resultVelocityRcs applies to the ball
        // convert to motor setpoint
        // this used to be called TokyoDrift
        data.resultVelocityRcs.x += data.resultVelocityRcs.phi * data.ppConfig.forwardDriving.radiusRobotToBall;

        // unshift coordinates
        // - SPG 'feedforward' data update
        auto offset = Position2D(0.0, -data.ppConfig.forwardDriving.radiusRobotToBall, 0.0);
        data.targetPositionFcs = addRcsToFcs(offset, data.targetPositionFcs);
        data.currentPositionFcs = addRcsToFcs(offset, data.currentPositionFcs);

        // for SPG
        data.previousPositionSetpointFcs = addRcsToFcs(offset, data.previousPositionSetpointFcs);
    }
}

