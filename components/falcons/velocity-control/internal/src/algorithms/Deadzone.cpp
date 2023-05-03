/*
 * Deadzone.cpp
 *
 *  Created on: January, 2020
 *      Author: Jan Feitsma
 */

#include "VelocityControlAlgorithms.hpp"


void Deadzone::execute(VelocityControlData &data)
{

    // apply Deadzone only when getting a position-based setpoint
    if (data.robotPosVelMoveType == robotPosVelEnum::POSVEL || data.robotPosVelMoveType == robotPosVelEnum::POS_ONLY)
    {

        // compare with tolerances
        bool xyOk = data.deltaPositionRcs.xy().size() < data.ppConfig.deadzone.toleranceXY;
        bool RzOk = fabs(data.deltaPositionRcs.phi) < data.ppConfig.deadzone.toleranceRz;

        // "deadzone" overrule: in case of partial convergence, set zero output
        // this is especially useful during tuning, to prevent a tiny Rz move setpoint interfering with XY
        data.deadzone[0] = false;
        data.deadzone[1] = false;
        data.deadzone[2] = false;
        if (data.ppConfig.deadzone.enabled)
        {
            if (xyOk)
            {
                data.resultVelocityRcs.x = 0.0;
                data.resultVelocityRcs.y = 0.0;
                data.deadzone[0] = true;
                data.deadzone[1] = true;
            }
            
            if (RzOk)
            {
                data.resultVelocityRcs.phi = 0.0;
                data.deadzone[2] = true;
            }

            if (!xyOk && !RzOk)
            {
            }
        }
        else
        {
        }
    }
    else
    {
    }
}

