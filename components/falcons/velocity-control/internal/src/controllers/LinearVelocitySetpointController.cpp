/*
 * LinearVelocitySetpointController.cpp
 *
 *  Created on: October, 2019
 *      Author: Jan Feitsma
 */

#include "VelocitySetpointControllers.hpp"


bool LinearVelocitySetpointController::calculate(VelocityControlData &data, Velocity2D &resultVelocity)
{

    Position2D deltaPosition; // in RCS or FCS depending on configuration

    if (data.vcSetpointConfig.coordinateSystem == CoordinateSystemEnum::RCS)
    {
        deltaPosition = data.deltaPositionRcs;
    }
    else
    {
        deltaPosition = data.deltaPositionFcs;
    }

    // XY
    Vector2D v = deltaPosition.xy() / data.dt;
    resultVelocity.x = v.x;
    resultVelocity.y = v.y;
    // Rz
    float dRz = deltaPosition.phi;
    resultVelocity.phi = dRz / data.dt;


    return true;
}

