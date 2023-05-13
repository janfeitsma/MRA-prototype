/*
 * VelocityControl.cpp
 *
 *  Created on: May 2023
 *      Author: Jan Feitsma
 */

// own package
#include "VelocityControl.hpp"
#include "VelocitySetpointControllers.hpp"
#include "VelocityControlAlgorithms.hpp"


VelocityControl::VelocityControl()
{
}

VelocityControl::~VelocityControl()
{
}

void VelocityControl::iterate()
{
    // this function assumes all required inputs (Inputs, Params, State) are set under "data"
    // each algorithm performs some operation on the data
    // the sequence of algorithms matters and is managed here, based on configuration

    std::vector<VelocityControlAlgorithm *> algorithms;

    // check that input contains valid data (no z,rx,ry)
    // determine control mode: POSVEL, POS_ONLY or VEL_ONLY
    // set internal variables based on inputs, state, params
    algorithms.push_back(new CheckPrepareInputs());

    // determine the limits to use based on configuration and input motion profile
    algorithms.push_back(new ConfigureLimits());

    // prevent runaway setpoints by ensuring that last call was recent enough
//    algorithms.push_back(new Watchdog());

    // prevent wasting energy by responding to very small setpoints
    algorithms.push_back(new Deadzone());

    // to enable dribbling, limits should apply to ball, not robot
    algorithms.push_back(new ShiftBallOffset());

    // velocity control (in FCS or RCS, depends on configuration)
    // for now: always SPG (SetPointGenerator) using Reflexxes Type II library
    algorithms.push_back(new SelectVelocityController());
    algorithms.push_back(new CalculateVelocity());

    // SPG intrinsically ensures limits are satisfied
    // if the controller would be PID or Linear, then ApplyLimits would be needed
    //algorithms.push_back(new ApplyLimits());

    // to enable dribbling, limits should apply to ball, not robot
    algorithms.push_back(new UnShiftBallOffset());

    // finally, set output data, write state variables to be used in next iteration
    algorithms.push_back(new SetOutputsPrepareNext());

    // execute the sequence of algorithms
    for (auto it = algorithms.begin(); it != algorithms.end(); ++it)
    {
        if (!data.done)
        {
            (*it)->execute(data);
        }
    }
}