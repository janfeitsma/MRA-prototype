/*
 * VelocitySetpointControllers.hpp
 *
 *  Created on: October, 2020
 *      Author: Erik Kouters
 */

#ifndef VELOCITYCONTROL_VELOCITYSETPOINTCONTROLLERS_HPP_
#define VELOCITYCONTROL_VELOCITYSETPOINTCONTROLLERS_HPP_

// own package
#include "AbstractVelocitySetpointController.hpp"
//#include "facilities/PIDController.hpp" // PID not supported in MRA context


// controller definitions, each implementation has its own file

class StopVelocitySetpointController : public AbstractVelocitySetpointController
{
public:
    bool calculate(VelocityControlData &data);
};

// not useful for robot, but works OK for simulation
class LinearVelocitySetpointController : public AbstractVelocitySetpointController
{
public:
    bool calculate(VelocityControlData &data);
};

// SetPointGenerator, using Reflexxes motion control library
#include "SPGVelocitySetpointController.hpp"

/* // PID not supported in MRA context
// used on Falcons robot up to and including 2019
class PIDVelocitySetpointController : public AbstractVelocitySetpointController
{
public:
    PIDVelocitySetpointController(MotionPIDConfig const &config) { _config = config; }
    bool calculate(VelocityControlData &data, Velocity2D &resultVelocity);
private:
    MotionPIDConfig _config;
    // controllers need to remain state in between ticks, for integral term
    PIDController _controllerX;
    PIDController _controllerY;
    PIDController _controllerRz;
};
*/

#endif

