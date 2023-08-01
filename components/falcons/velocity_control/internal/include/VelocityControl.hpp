/*
 * VelocityControl.hpp
 *
 *  Created on: May 2023
 *      Author: Jan Feitsma
 */

#ifndef VELOCITYCONTROL_HPP_
#define VELOCITYCONTROL_HPP_


#include "VelocityControlData.hpp"
#include "VelocityControlAlgorithms.hpp"


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

private:
    std::vector<std::shared_ptr<VelocityControlAlgorithm>> algorithms;
    void setup();
    void add_algorithm(std::shared_ptr<VelocityControlAlgorithm> alg, bool unskippable = false);
};

} // namespace MRA::internal::FVC

#endif

