/*
 * CheckPrepareInputs.cpp
 *
 *  Created on: May 2023
 *      Author: Jan Feitsma
 */

#include "VelocityControlAlgorithms.hpp"
#include "VelocityControlExceptions.hpp"


void CheckPrepareInputs::execute(VelocityControlData &data)
{
    // check worldstate: is robot active, is it properly filled, ...
    checkWorldState(data);

    // check target setpoint: is there enough information, is it valid, return the requested control mode
    data.controlMode = checkTargetSetpoint(data);

    // set internal data variables
    setInternalVariables(data);
}

void CheckPrepareInputs::checkWorldState(VelocityControlData &data)
{
    // stop further processing if robot is set to inactive
    if (!data.input.worldstate().robot().active())
    {
        data.done = true;
        //throw VelocityControlExceptions::RobotInactive(__FILE__, __LINE__);
        return;
    }

    // check that robot position and velocity are set
    // (protobuf v3 does not allow checks on scalar level, it is valid to omit, in which case zeros are assumed)
    if (!data.input.worldstate().robot().has_position()) throw VelocityControlExceptions::IncompleteInput(__FILE__, __LINE__, "robot position");
    if (!data.input.worldstate().robot().has_velocity()) throw VelocityControlExceptions::IncompleteInput(__FILE__, __LINE__, "robot velocity");

    // checks on target setpoint are done in another function
}

ControlModeEnum CheckPrepareInputs::checkTargetSetpoint(VelocityControlData &data)
{
    // check if target position and/or velocity are set
    ControlModeEnum result = ControlModeEnum::INVALID;
    if (data.input.setpoint().has_position())
    {
        if (data.input.setpoint().has_velocity())
        {
            result = ControlModeEnum::POSVEL;
        }
        else
        {
            result = ControlModeEnum::POS_ONLY;
        }
    }
    else if (data.input.setpoint().has_velocity())
    {
        result = ControlModeEnum::VEL_ONLY;
    }
    else
    {
        data.done = true;
        return result;
    }

    // check for invalid dimensions -- robot cannot fly (yet ;))
    if (data.input.setpoint().position().z() != 0)  throw VelocityControlExceptions::UnsupportedDimension(__FILE__, __LINE__, "setpoint position.z");
    if (data.input.setpoint().position().rx() != 0) throw VelocityControlExceptions::UnsupportedDimension(__FILE__, __LINE__, "setpoint position.rx");
    if (data.input.setpoint().position().ry() != 0) throw VelocityControlExceptions::UnsupportedDimension(__FILE__, __LINE__, "setpoint position.ry");
    if (data.input.setpoint().velocity().z() != 0)  throw VelocityControlExceptions::UnsupportedDimension(__FILE__, __LINE__, "setpoint velocity.z");
    if (data.input.setpoint().velocity().rx() != 0) throw VelocityControlExceptions::UnsupportedDimension(__FILE__, __LINE__, "setpoint velocity.rx");
    if (data.input.setpoint().velocity().ry() != 0) throw VelocityControlExceptions::UnsupportedDimension(__FILE__, __LINE__, "setpoint velocity.ry");

    return result;
}

void CheckPrepareInputs::setInternalVariables(VelocityControlData &data)
{
    data.currentPositionFcs = MRA::Geometry::Position(data.input.worldstate().robot().position());
    data.currentVelocityFcs = MRA::Geometry::Velocity(data.input.worldstate().robot().velocity());
    data.targetPositionFcs  = MRA::Geometry::Position(data.input.setpoint().position());
    data.targetVelocityFcs  = MRA::Geometry::Velocity(data.input.setpoint().velocity());

    data.previousPositionSetpointFcs = MRA::Geometry::Position(data.state.positionsetpointfcs());
    data.previousVelocitySetpointFcs = MRA::Geometry::Velocity(data.state.velocitysetpointfcs());
}

