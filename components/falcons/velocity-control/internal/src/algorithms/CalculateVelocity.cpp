/*
 * CalculateVelocity.cpp
 *
 *  Created on: July, 2019
 *      Author: Jan Feitsma
 */

#include "VelocityControlAlgorithms.hpp"


void CalculateVelocity::execute(VelocityControlData &data)
{
    // check that some controller is configured
    assert(data.controller != NULL);

    // call, check result
    // the controller should write to data.resultVelocityRcs
    bool success = data.controller->calculate(data);
    if (!success)
    {
        throw std::runtime_error("something went wrong in velocity controller");
    }
}

