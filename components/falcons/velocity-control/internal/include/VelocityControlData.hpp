/*
 * VelocityControlData.hpp
 *
 *  Created on: Oct, 2020
 *      Author: Erik Kouters
 */

#ifndef VELOCITYCONTROLDATA_HPP_
#define VELOCITYCONTROLDATA_HPP_

// MRA-libraries
#include "MRAbridge.hpp"


struct VelocityControlData
{
    MRA_timestamp      timestamp;
    MRA_InputType      input;
    MRA_ParamsType     config;
    MRA_StateType      state;
    MRA_LocalType      diag;
    MRA_OutputType     output;
};

#endif

