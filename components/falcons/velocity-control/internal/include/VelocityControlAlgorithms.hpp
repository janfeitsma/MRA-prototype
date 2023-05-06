/*
 * VelocityControlAlgorithms.hpp
 *
 *  Created on: Oct, 2020
 *      Author: Erik Kouters
 */

#ifndef VELOCITYCONTROL_ALGORITHMS_HPP_
#define VELOCITYCONTROL_ALGORITHMS_HPP_

#include "VelocityControlData.hpp"
#include "AbstractVelocitySetpointController.hpp"

/*!
 * \brief is the abstract class that each VelocityControl algorithm should inherit.
 *
 */
class VelocityControlAlgorithm
{
public:
    VelocityControlAlgorithm() {}

    virtual void execute(VelocityControlData &data) = 0;
};


// algorithm definitions, each implementation has its own file

// check that input contains valid data (no z,rx,ry)
// determine control mode: POSVEL, POS_ONLY or VEL_ONLY
class CheckInputs : public VelocityControlAlgorithm
{
    void execute(VelocityControlData &data);
};

// determine the limits to use based on configuration and input motion profile
class ConfigureLimits : public VelocityControlAlgorithm
{
    void execute(VelocityControlData &data);
};

// prevent runaway setpoints by ensuring that last call was recent enough
class Watchdog : public VelocityControlAlgorithm
{
    void execute(VelocityControlData &data);
};

// to enable dribbling, limits should apply to ball, not robot
class ShiftBallOffset : public VelocityControlAlgorithm
{
    void execute(VelocityControlData &data);
};
class UnShiftBallOffset : public VelocityControlAlgorithm
{
    void execute(VelocityControlData &data);
};

// velocity control (in FCS or RCS, depends on configuration)
// for now: always SPG (SetPointGenerator) using Reflexxes Type II library
class SelectVelocityController : public VelocityControlAlgorithm
{
    void execute(VelocityControlData &data);
};
class CalculateVelocity : public VelocityControlAlgorithm
{
    void execute(VelocityControlData &data);
};

// prevent wasting energy by responding to very small setpoints
class Deadzone : public VelocityControlAlgorithm
{
    void execute(VelocityControlData &data);
};

// write state variables to be used in next iteration
class PrepareNext : public VelocityControlAlgorithm
{
    void execute(VelocityControlData &data);
};

#endif
