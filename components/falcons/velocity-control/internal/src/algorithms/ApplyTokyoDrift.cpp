/*
 * ApplyTokyoDrift.cpp
 *
 *  Created on: Sep, 2021
 *      Author: Erik Kouters
 */

#include "VelocityControlAlgorithms.hpp"
#include "facilities/vcgeometry.hpp"


void ApplyTokyoDrift::execute(VelocityControlData &data)
{

    if (data.robot.hasBall && fabs(data.deltaPositionRcs.phi) > data.ppConfig.tokyoDrift.toleranceRz)
    {
        // resultVelocityRcs applies to the ball
        // convert to motor setpoint
        data.resultVelocityRcs.x += data.resultVelocityRcs.phi * data.ppConfig.forwardDriving.radiusRobotToBall;
    }
}

