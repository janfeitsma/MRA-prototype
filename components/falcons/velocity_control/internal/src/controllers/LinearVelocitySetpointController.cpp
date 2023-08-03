/*
 * LinearVelocitySetpointController.cpp
 *
 *  Created on: October, 2019
 *      Author: Jan Feitsma
 */

#include "VelocitySetpointControllers.hpp"


bool LinearVelocitySetpointController::calculate(VelocityControlData &data)
{
    // data should be filled in beforehand

    // prevent division by zero
    if (data.config.dt() == 0) return false;

    // calculate delta in FCS
    Position2D deltaPosition = data.targetPositionFcs - data.currentPositionFcs;

    // divide by dt
    Velocity2D resultVelocityFcs(deltaPosition / data.config.dt());

    // transform to RCS
    data.resultVelocityRcs = resultVelocityFcs.transformFcsToRcs(data.currentPositionFcs);

    // typically ApplyLimits should be called afterwards

    return true;
}

