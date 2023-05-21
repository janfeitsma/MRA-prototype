/*
 * SelectVelocityController.cpp
 *
 *  Created on: December, 2019
 *      Author: Jan Feitsma
 */

#include "VelocityControlAlgorithms.hpp"
#include "AbstractVelocitySetpointController.hpp"
#include "VelocitySetpointControllers.hpp"


void SelectVelocityController::execute(VelocityControlData &data)
{
    data.controller = std::shared_ptr<AbstractVelocitySetpointController>(new SPGVelocitySetpointController());
}

