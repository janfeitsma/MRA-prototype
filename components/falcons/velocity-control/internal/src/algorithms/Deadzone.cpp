/*
 * Deadzone.cpp
 *
 *  Created on: January, 2020
 *      Author: Jan Feitsma
 */

#include "VelocityControlAlgorithms.hpp"
#include <cmath>


void Deadzone::execute(VelocityControlData &data)
{
    // do nothing if configuration enabled==false
    auto config = data.config.deadzone();
    if (!config.enabled()) return;

    // apply Deadzone only when getting a position-based setpoint
    if (data.controlMode == ControlModeEnum::POSVEL || data.controlMode == ControlModeEnum::POS_ONLY)
    {
        Position2D delta = data.targetPositionFcs - data.currentPositionFcs;

        // compare with tolerances
        bool xyOk = delta.size() < config.tolerancexy();
        bool RzOk = fabs(delta.rz) < config.tolerancerz();

        if (xyOk && RzOk)
        {
            data.done = true;
            // no need to run SPG etc.
        }
    }
}

