/*
 * CalculateAccelerating.cpp
 *
 *  Created on: December, 2019
 *      Author: Jan Feitsma
 */

#include "VelocityControlAlgorithms.hpp"


void CalculateAccelerating::execute(VelocityControlData &data)
{

    // calculate current velocity in RCS
    Velocity2D c = data.currentVelocityFcs;
    c = c.transform_fcs2rcs(data.currentPositionFcs);

    // calculate acceleration based on setpoints (real acceleration typically lags behind a bit)
    auto a = (data.resultVelocityRcs - data.previousVelocityRcs) / data.dt;


    // take inproduct of acceleration with current velocity to determine if robot is accelerating or not
    data.isAccelerating[0] = (a.x * c.x >= data.currentMotionTypeConfig.limits.accThresholdX);
    data.isAccelerating[1] = (a.y * c.y >= data.currentMotionTypeConfig.limits.accThresholdY);
    data.isAccelerating[2] = (a.phi * c.phi >= data.currentMotionTypeConfig.limits.accThresholdRz);

}

