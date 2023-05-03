/*
 * SelectVelocityController.cpp
 *
 *  Created on: December, 2019
 *      Author: Jan Feitsma
 */

#include "VelocityControlAlgorithms.hpp"


void SelectVelocityController::execute(VelocityControlData &data)
{

    // get delta
    float distanceToSubTarget = data.deltaPositionFcs.xy().size();

    // switch between long and short stroke
    if (distanceToSubTarget > data.currentMotionTypeConfig.velocityControllers.threshold)
    {
        data.shortStroke = false;
        data.vcSetpointConfig = data.currentMotionTypeConfig.velocityControllers.longStroke;
    }
    else
    {
        data.shortStroke = true;
        data.vcSetpointConfig = data.currentMotionTypeConfig.velocityControllers.shortStroke;
    }
}

