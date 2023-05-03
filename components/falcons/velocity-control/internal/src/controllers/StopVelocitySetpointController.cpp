/*
 * StopVelocitySetpointController.cpp
 *
 *  Created on: October, 2019
 *      Author: Jan Feitsma
 */

#include "VelocitySetpointControllers.hpp"


bool StopVelocitySetpointController::calculate(VelocityControlData &data, Velocity2D &resultVelocity)
{
    TRACE_FUNCTION("");
    resultVelocity.x = 0.0;
    resultVelocity.y = 0.0;
    resultVelocity.phi = 0.0;
    return true;
}

