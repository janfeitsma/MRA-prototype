/*
 * AbstractVelocitySetpointController.hpp
 *
 *  Created on: October 2020
 *      Author: Erik Kouters
 */

#ifndef VELOCITYCONTROL_ABSTRACTVELOCITYSETPOINTCONTROLLER_HPP_
#define VELOCITYCONTROL_ABSTRACTVELOCITYSETPOINTCONTROLLER_HPP_


// forward declaration
class VelocityControlData;


// abstract base class for specific controllers, like PID, TokyoDrift, Linear, ...

class AbstractVelocitySetpointController
{
public:
    AbstractVelocitySetpointController() {};
    virtual ~AbstractVelocitySetpointController() {};

    // modify resulting data.resultVelocityRcs
    // return success
    virtual bool calculate(VelocityControlData &data) = 0;

};

#endif

