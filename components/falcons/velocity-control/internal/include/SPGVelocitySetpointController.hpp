/*
 * SPGVelocitySetpointController.hpp
 *
 *  Created on: Oct, 2020
 *      Author: Erik Kouters
 */

#ifndef VELOCITYCONTROL_SPGVELOCITYSETPOINTCONTROLLER_HPP_
#define VELOCITYCONTROL_SPGVELOCITYSETPOINTCONTROLLER_HPP_

// own package
#include "VelocitySetpointControllers.hpp"

// MRA-libraries
#include "MRAbridge.hpp"

// external
#include <ReflexxesAPI.h>


struct SpgLimits
{
    float vx;
    float vy;
    float vRz;
    float ax;
    float ay;
    float aRz;
};

class SPGVelocitySetpointController : public AbstractVelocitySetpointController
{
public:
    SPGVelocitySetpointController(SpgConfig const &config);
    ~SPGVelocitySetpointController();
    bool calculate(VelocityControlData &data, Velocity2D &resultVelocity);

private:
    SpgConfig _config;

    bool calculateSPG(VelocityControlData& data, SpgLimits const &spgLimits, Position2D& resultPosition, Velocity2D &resultVelocity);
    bool isDofAccelerating(const VelocityControlData &data, const Velocity2D& resultVelocity, int dof, float threshold);

    // Position SPG
    bool calculatePosXYRzPhaseSynchronized(VelocityControlData& data, SpgLimits const &spgLimits, Position2D& resultPosition, Velocity2D &resultVelocity);
    bool calculatePosXYPhaseSynchronized(VelocityControlData& data, SpgLimits const &spgLimits, Position2D& resultPosition, Velocity2D &resultVelocity);
    bool calculatePosRzNonSynchronized(VelocityControlData& data, SpgLimits const &spgLimits, Position2D& resultPosition, Velocity2D &resultVelocity);

    // Velocity SPG
    bool calculateVelXYRzPhaseSynchronized(VelocityControlData& data, SpgLimits const &spgLimits, Position2D& resultPosition, Velocity2D &resultVelocity);

    // data stored for open loop
    Position2D _deltaPositionRCS;
    Velocity2D _currentVelocityRCS;
    Velocity2D _targetVelocityRCS;

    // SPG with Velocity setpoint

};

#endif

