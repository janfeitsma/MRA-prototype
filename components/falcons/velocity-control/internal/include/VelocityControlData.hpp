/*
 * VelocityControlData.hpp
 *
 *  Created on: May 2023
 *      Author: Jan Feitsma
 */

#ifndef VELOCITYCONTROLDATA_HPP_
#define VELOCITYCONTROLDATA_HPP_

// MRA libraries
#include "MRAbridge.hpp"

// forward declaration
class AbstractVelocitySetpointController;

namespace MRA::internal::FVC
{

// this struct is used (r/w) by every algorithm
struct VelocityControlData
{
    // MRA interface
    MRA_timestamp      timestamp;
    MRA_InputType      input;
    MRA_ParamsType     config;
    MRA_StateType      state;
    MRA_LocalType      diag;
    MRA_OutputType     output;

    // while running the sequence of algorithms, this flag may be raised
    bool done;

    // which controller to use, determined by the SelectVelocityController algorithm
    std::shared_ptr<AbstractVelocitySetpointController> controller;

    // set the limits based on full configuration and input
    MRA::FalconsVelocityControl::Limits limits;

    // internal variables
    MRA::FalconsVelocityControl::ControlModeEnum controlMode;
    Position2D currentPositionFcs;
    Velocity2D currentVelocityFcs;
    Position2D targetPositionFcs;
    Velocity2D targetVelocityFcs;
    Position2D previousPositionSetpointFcs;
    Velocity2D previousVelocitySetpointFcs;

    // result
    Velocity2D resultVelocityRcs;

};

} // namespace MRA::internal::FVC

using namespace MRA::internal::FVC; //HACK

#endif

