/*
 * VelocityControl.hpp
 *
 *  Created on: Oct, 2020
 *      Author: Erik Kouters
 */

#ifndef VELOCITYCONTROL_HPP_
#define VELOCITYCONTROL_HPP_


// data struct
#include "VelocityControlData.hpp"


class VelocityControl
{
public:
    VelocityControl();
    ~VelocityControl();

    void iterate();

public:
    // having these public is convenient for test suite
    VelocityControlData data;
/* TODO below
private:
    // helper functions
    void getInputs();
    void setupVelocitySetpointController();
    AbstractVelocitySetpointController *getVelocitySetpointController();
    AbstractVelocitySetpointController *setupAndGetVelocitySetpointController();

    AbstractVelocitySetpointController *_velocitySetpointController = NULL;
    VelocitySetpointControllerTypeEnum  _currentVelocitySetpointControllerType;
*/
};

#endif

