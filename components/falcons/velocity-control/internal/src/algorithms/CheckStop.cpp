/*
 * CheckStop.cpp
 *
 *  Created on: May, 2023
 *      Author: Jan Feitsma
 */

#include "VelocityControlAlgorithms.hpp"


void CheckStop::execute(VelocityControlData &data)
{
    if (data.controlMode != MRA::FalconsVelocityControl::ControlModeEnum::VEL_ONLY)
    {
        return;
    }
    if (data.targetVelocityFcs.x == 0.0 && data.targetVelocityFcs.y == 0.0 && data.targetVelocityFcs.rz == 0.0)
    {
        // wipe state to prevent SPG continuing full throttle after stop
        data.previousPositionSetpointFcs.reset();
        data.previousVelocitySetpointFcs.reset();

        // no need to set output velocity to zero, it is already initialized as such
        data.done = true;
    }
}

