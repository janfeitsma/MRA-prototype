/*
 * VelocityControl.hpp
 *
 *  Created on: May 2023
 *      Author: Jan Feitsma
 */

#ifndef VELOCITYCONTROL_HPP_
#define VELOCITYCONTROL_HPP_


// data struct
#include "VelocityControlData.hpp"


namespace MRA::internal::FVC
{

class VelocityControl
{
public:
    VelocityControl();
    ~VelocityControl();

    void iterate();

public:
    // having these public is convenient for test suite
    VelocityControlData data{};

};

} // namespace MRA::internal::FVC

#endif

