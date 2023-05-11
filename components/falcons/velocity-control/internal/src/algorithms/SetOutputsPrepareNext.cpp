/*
 * SetOutputsPrepareNext.cpp
 *
 *  Created on: May 2023
 *      Author: Jan Feitsma
 */

#include "VelocityControlAlgorithms.hpp"


void SetOutputsPrepareNext::execute(VelocityControlData &data)
{
    data.output.mutable_velocity()->set_x(data.resultVelocityRcs.x);
    data.output.mutable_velocity()->set_y(data.resultVelocityRcs.y);
    data.output.mutable_velocity()->set_rz(data.resultVelocityRcs.rz);
}

