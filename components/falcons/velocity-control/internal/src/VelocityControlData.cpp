/*
 * VelocityControlData.cpp
 *
 *  Created on: July, 2019
 *      Author: Jan Feitsma
 */

#include "VelocityControlData.hpp"
#include "generated_enum2str.hpp"


void VelocityControlData::reset()
{
    // clear calculation results and internal data; inputs are overridden at start of iteration
    // make sure all diagnostics related items are cleared here, otherwise you might end up diagnosing data from an older iteration
    deltaPositionFcs = Position2D(0.0, 0.0, 0.0);
    deltaPositionRcs = Position2D(0.0, 0.0, 0.0);
    resultVelocityRcs = Velocity2D(0.0, 0.0, 0.0);
    accelerationRcs = pose();
    for (int dof = 0; dof < 3; ++dof)
    {
        isAccelerating[dof] = false;
        accelerationClipping[dof] = false;
        deadzone[dof] = false;
    }
    done = false;
    motionType = motionTypeEnum::INVALID;
}

void VelocityControlData::configureLimits()
{
    auto it = vcConfig.motionTypes.find( enum2str(motionType) );
    if (it == vcConfig.motionTypes.end())
    {
        std::stringstream str;
        str << "motionType '" << enum2str(motionType) << "' (" << (int)motionType << ") not found in VelocityControl configuration";
        throw std::runtime_error(str.str());
    }
    currentMotionTypeConfig = vcConfig.motionTypes.at( enum2str(motionType) );

}

