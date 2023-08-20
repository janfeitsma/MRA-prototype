/*
 * SPGVelocitySetpointController.cpp
 *
 *  Created on: November, 2019
 *      Author: Jan Feitsma
 */

// TODO: reduce excessive code duplication, improve maintainability

#include "SPGVelocitySetpointController.hpp"

#include <algorithm>

SPGVelocitySetpointController::SPGVelocitySetpointController()
{
}

SPGVelocitySetpointController::~SPGVelocitySetpointController()
{
}

bool SPGVelocitySetpointController::calculate(VelocityControlData &data)
{
    // Type II Reflexxes Motion Library
    // note that this variant does not support jerk control

    // in order to achieve higher deceleration, a layer is added around the SPG algorithm
    // we start with (aggressive) deceleration limits, so the robot is normally going to be in time for braking
    // and we get a good estimate of velocity and acceleration setpoint
    // then, if robot _seems_ to be accelerating, corresponding limits are used in a recalculation
    SpgLimits spgLimits;
    spgLimits.vx = data.limits.maxvel().x();
    spgLimits.vy = data.limits.maxvel().yforward();
    spgLimits.vRz = data.limits.maxvel().rz();
    spgLimits.ax = data.limits.maxdec().x();
    spgLimits.ay = data.limits.maxdec().y();
    spgLimits.aRz = data.limits.maxdec().rz();

    // For position, finding a weighted average on the Rz is not trivial when the angles are around the boundary of [0, 2pi].
    // To solve this, rotate both currentPosFCS.Rz and previousPosSetpointFCS.Rz towards currentPosFCS.Rz.
    // This means currentPosFCS.Rz = 0, and previousPosSetpointFCS.Rz is the difference to currentPosFCS.Rz.
    // At this point, take the weighted factor from previousPosSetpointFCS.Rz -> deltaAngle projected between [-pi, pi]
    // This weighted factor can be rotated back from currentPosFCS.Rz.
    // The final average angle between the two is then currentPosFCS.Rz + deltaAngle 

    float w_pos = data.config.spg().weightfactorclosedlooppos();
    double deltaRz = MRA::Geometry::wrap_pi(data.previousPositionSetpointFcs.rz - data.currentPositionFcs.rz) * w_pos;
    double weightedRz = data.currentPositionFcs.rz + deltaRz;

    Position2D weightedCurrentPositionFCS = data.currentPositionFcs * w_pos + data.previousPositionSetpointFcs * (1.0-w_pos);
    weightedCurrentPositionFCS.rz = weightedRz;
    float w_vel = data.config.spg().weightfactorclosedloopvel();
    Velocity2D weightedCurrentVelocityFCS = data.currentVelocityFcs * w_vel + data.previousVelocitySetpointFcs * (1.0-w_vel);

    _deltaPositionRCS = Position2D(data.targetPositionFcs).transformFcsToRcs(weightedCurrentPositionFCS);
    _deltaPositionRCS.rz = MRA::Geometry::wrap_pi(data.targetPositionFcs.rz - weightedCurrentPositionFCS.rz);
    _currentVelocityRCS = Velocity2D(weightedCurrentVelocityFCS).transformFcsToRcs(weightedCurrentPositionFCS);
    _targetVelocityRCS = Velocity2D(data.targetVelocityFcs).transformFcsToRcs(weightedCurrentPositionFCS);


    Position2D resultPosition;
    Velocity2D resultVelocity;
    bool result = calculateSPG(data, spgLimits, resultPosition, resultVelocity);

    bool recalculate = false;
    if (isDofAccelerating(data, resultVelocity, 0, data.limits.accthreshold().x()))
    {
        recalculate = true;
        spgLimits.ax = data.limits.maxacc().x();
    }
    if (isDofAccelerating(data, resultVelocity, 1, data.limits.accthreshold().y()))
    {
        recalculate = true;
        spgLimits.ay = (resultVelocity.y < 0.0) ? data.limits.maxacc().ybackward() : data.limits.maxacc().yforward();
    }
    if (isDofAccelerating(data, resultVelocity, 2, data.limits.accthreshold().rz()))
    {
        recalculate = true;
        spgLimits.aRz = data.limits.maxacc().rz();
    }
    if (resultVelocity.y < 0.0)
    {
        recalculate = true;
        spgLimits.vy = data.limits.maxvel().ybackward();
    }


    // recalculate?
    if (recalculate)
    {
        result = calculateSPG(data, spgLimits, resultPosition, resultVelocity);
    }


    // check if motor limits are not exceeded.
    /* disabled - in MRA context, we must not depend on vtClient
    for(int i = 0; i < 10; i++)
    {
        if (data.vtClient.exceedsMotorLimits( pose(resultVelocity.x, resultVelocity.y, resultVelocity.rz) ))
        {
            // reduce limits by 20% to see if we can find setpoints which do not exceed the motor limits
            spgLimits.vx *= 0.8;
            spgLimits.vy *= 0.8;
            spgLimits.vRz *= 0.8;
            result = calculateSPG(data, spgLimits, resultPosition, resultVelocity);
        }
        else
        {
            break;
        }
    }
    */

    // Done -- store output and values for next iteration
    data.resultVelocityRcs = resultVelocity;

    // Store previousPositionSetpointFcs for open loop control
    Position2D tmpPos = resultPosition;
    data.previousPositionSetpointFcs = tmpPos.transformRcsToFcs(weightedCurrentPositionFCS);
    // ???data.previousPositionSetpointFcs.rz += M_PI_2;
    // ??? data.previousPositionSetpointFcs.rz = project_angle_0_2pi(data.previousPositionSetpointFcs.rz);

    // Store previousVelocitySetpointFcs for open loop control
    Velocity2D tmpVel = resultVelocity;
    data.previousVelocitySetpointFcs = tmpVel.transformRcsToFcs(weightedCurrentPositionFCS);

    return result;
}

bool SPGVelocitySetpointController::isDofAccelerating(const VelocityControlData &data, const Velocity2D& resultVelocity, int dof, float threshold)
{
    // To check if a DOF is accelerating, we should look if the _currentVelocityRCS -> resultVelocity is "moving away from zero".

    std::vector<double> currentVelocity;
    currentVelocity.push_back(_currentVelocityRCS.x);
    currentVelocity.push_back(_currentVelocityRCS.y);
    currentVelocity.push_back(_currentVelocityRCS.rz);

    std::vector<double> newVelocity;
    newVelocity.push_back(resultVelocity.x);
    newVelocity.push_back(resultVelocity.y);
    newVelocity.push_back(resultVelocity.rz);


    if (currentVelocity.at(dof) < 0.0)
    {
        // newVelocity - currentVelocity < threshold
        // e.g., -2.0 - -1.0 = -1.0 < (-threshold)
        return (newVelocity.at(dof) - currentVelocity.at(dof)) < (-threshold);
    }
    else if (currentVelocity.at(dof) > 0.0)
    {
        // newVelocity - currentVelocity > threshold
        // e.g., 2.0 - 1.0 = 1.0 > threshold
        return (newVelocity.at(dof) - currentVelocity.at(dof)) > threshold;
    }
    else
    {
        // currentVelocity == 0.0
        return abs(newVelocity.at(dof)) > threshold;
    }

    return true;
}




bool SPGVelocitySetpointController::calculateSPG(VelocityControlData& data, SpgLimits const &spgLimits, Position2D& resultPosition, Velocity2D &resultVelocity)
{
    bool result = false;
    if (data.controlMode != MRA::FalconsVelocityControl::ControlModeEnum::VEL_ONLY)
    {
        // POS_ONLY or POSVEL
        if (data.config.spg().synchronizerotation())
        {
            result = calculatePosXYRzPhaseSynchronized(data, spgLimits, resultPosition, resultVelocity);
        }
        else
        {
            result = calculatePosXYPhaseSynchronized(data, spgLimits, resultPosition, resultVelocity);
            result = calculatePosRzNonSynchronized(data, spgLimits, resultPosition, resultVelocity);
        }
    }
    else
    {
        // Reflexxes has a problem when velocity already reached
        double EPSILON = 1E-4;
        if (   abs(_currentVelocityRCS.x   - std::clamp(_targetVelocityRCS.x,   (double) -spgLimits.vx,  (double) spgLimits.vx))  < EPSILON
            && abs(_currentVelocityRCS.y   - std::clamp(_targetVelocityRCS.y,   (double) -spgLimits.vy,  (double) spgLimits.vy))  < EPSILON
            && abs(_currentVelocityRCS.rz  - std::clamp(_targetVelocityRCS.rz,  (double) -spgLimits.vRz, (double) spgLimits.vRz)) < EPSILON)
        {
            resultVelocity = Velocity2D(_currentVelocityRCS);
            return true;
        }
        else
        {
            result = calculateVelXYRzPhaseSynchronized(data, spgLimits, resultPosition, resultVelocity);
        }
    }
    return result;
}

bool SPGVelocitySetpointController::calculatePosXYRzPhaseSynchronized(VelocityControlData& data, const SpgLimits& spgLimits, Position2D& resultPosition, Velocity2D &resultVelocity)
{

    // initialize
    ReflexxesAPI                *RML = NULL;
    RMLPositionInputParameters  *IP = NULL;
    RMLPositionOutputParameters *OP = NULL;
    RMLPositionFlags            Flags;
    Flags.SynchronizationBehavior                  = RMLPositionFlags::PHASE_SYNCHRONIZATION_IF_POSSIBLE;
    Flags.BehaviorAfterFinalStateOfMotionIsReached = RMLPositionFlags::RECOMPUTE_TRAJECTORY;

    // construct Reflexxes objects
    int numberOfDOFs = 3; //X, Y, Rz
    RML = new ReflexxesAPI(numberOfDOFs, data.config.dt()); // degrees of freedom (x,y,Rz), nominal cycle time
    IP  = new RMLPositionInputParameters(numberOfDOFs);
    OP  = new RMLPositionOutputParameters(numberOfDOFs);

    // set-up the input parameters
    IP->CurrentPositionVector->VecData      [0] = 0.0; // instead of steering from current to target,
    IP->CurrentPositionVector->VecData      [1] = 0.0; // we steer from zero to delta, so we can better configure
    IP->CurrentPositionVector->VecData      [2] = 0.0; // controlling FCS or RCS

    IP->CurrentVelocityVector->VecData      [0] = _currentVelocityRCS.x;
    IP->CurrentVelocityVector->VecData      [1] = _currentVelocityRCS.y;
    IP->CurrentVelocityVector->VecData      [2] = _currentVelocityRCS.rz;

    IP->CurrentAccelerationVector->VecData  [0] = 0.0; // not relevant, due to limitation of TypeII library
    IP->CurrentAccelerationVector->VecData  [1] = 0.0;
    IP->CurrentAccelerationVector->VecData  [2] = 0.0;

    IP->MaxVelocityVector->VecData          [0] = spgLimits.vx;
    IP->MaxVelocityVector->VecData          [1] = spgLimits.vy;
    IP->MaxVelocityVector->VecData          [2] = spgLimits.vRz;

    IP->MaxAccelerationVector->VecData      [0] = spgLimits.ax;
    IP->MaxAccelerationVector->VecData      [1] = spgLimits.ay;
    IP->MaxAccelerationVector->VecData      [2] = spgLimits.aRz;

    IP->MaxJerkVector->VecData              [0] = 0.0; // not used in TypeII library
    IP->MaxJerkVector->VecData              [1] = 0.0;
    IP->MaxJerkVector->VecData              [2] = 0.0;

    IP->TargetPositionVector->VecData       [0] = _deltaPositionRCS.x;  // steering from currentPos = 0,
    IP->TargetPositionVector->VecData       [1] = _deltaPositionRCS.y;  // to targetPos = deltaPos
    IP->TargetPositionVector->VecData       [2] = _deltaPositionRCS.rz;

    IP->TargetVelocityVector->VecData       [0] = _targetVelocityRCS.x;
    IP->TargetVelocityVector->VecData       [1] = _targetVelocityRCS.y;
    IP->TargetVelocityVector->VecData       [2] = _targetVelocityRCS.rz;

    IP->SelectionVector->VecData            [0] = true;
    IP->SelectionVector->VecData            [1] = true;
    IP->SelectionVector->VecData            [2] = true;

    // call the Reflexxes Online Trajectory Generation algorithm
    int r1 = RML->RMLPosition(*IP, OP, Flags);
    if (r1 < 0)
    {
        // error state
        return false;
    }

    // output parameters have been evaluated at first tick (data.config.dt())
    // latency correction: evaluate the trajectory at some offset
    double timeOffset = data.config.dt() + data.config.spg().latencyoffset();
    RML->RMLPositionAtAGivenSampleTime(timeOffset, OP);

    // convert outputs
    resultPosition.x   = OP->NewPositionVector->VecData[0];
    resultPosition.y   = OP->NewPositionVector->VecData[1];
    resultPosition.rz  = OP->NewPositionVector->VecData[2];

    resultVelocity.x   = OP->NewVelocityVector->VecData[0];
    resultVelocity.y   = OP->NewVelocityVector->VecData[1];
    resultVelocity.rz  = OP->NewVelocityVector->VecData[2];

    // Diag data
    /*TODO
    data.spgCurrentPosition.x  = IP->CurrentPositionVector->VecData[0];
    data.spgCurrentPosition.y  = IP->CurrentPositionVector->VecData[1];
    data.spgCurrentPosition.Rz = IP->CurrentPositionVector->VecData[2];

    data.spgCurrentVelocity.x  = IP->CurrentVelocityVector->VecData[0];
    data.spgCurrentVelocity.y  = IP->CurrentVelocityVector->VecData[1];
    data.spgCurrentVelocity.Rz = IP->CurrentVelocityVector->VecData[2];

    data.spgMaxVelocity.x  = IP->MaxVelocityVector->VecData[0];
    data.spgMaxVelocity.y  = IP->MaxVelocityVector->VecData[1];
    data.spgMaxVelocity.Rz = IP->MaxVelocityVector->VecData[2];

    data.spgMaxAcceleration.x  = IP->MaxAccelerationVector->VecData[0];
    data.spgMaxAcceleration.y  = IP->MaxAccelerationVector->VecData[1];
    data.spgMaxAcceleration.Rz = IP->MaxAccelerationVector->VecData[2];

    data.spgTargetPosition.x  = IP->TargetPositionVector->VecData[0];
    data.spgTargetPosition.y  = IP->TargetPositionVector->VecData[1];
    data.spgTargetPosition.Rz = IP->TargetPositionVector->VecData[2];

    data.spgTargetVelocity.x  = IP->TargetVelocityVector->VecData[0];
    data.spgTargetVelocity.y  = IP->TargetVelocityVector->VecData[1];
    data.spgTargetVelocity.Rz = IP->TargetVelocityVector->VecData[2];

    data.spgNewPosition.x  = OP->NewPositionVector->VecData[0];
    data.spgNewPosition.y  = OP->NewPositionVector->VecData[1];
    data.spgNewPosition.Rz = OP->NewPositionVector->VecData[2];

    data.spgNewVelocity.x  = OP->NewVelocityVector->VecData[0];
    data.spgNewVelocity.y  = OP->NewVelocityVector->VecData[1];
    data.spgNewVelocity.Rz = OP->NewVelocityVector->VecData[2];
    */

    // cleanup
    delete RML;
    delete IP;
    delete OP;

    return true;
}

bool SPGVelocitySetpointController::calculatePosXYPhaseSynchronized(VelocityControlData& data, SpgLimits const &spgLimits, Position2D& resultPosition, Velocity2D &resultVelocity)
{

    // initialize
    ReflexxesAPI                *RML = NULL;
    RMLPositionInputParameters  *IP = NULL;
    RMLPositionOutputParameters *OP = NULL;
    RMLPositionFlags            Flags;
    Flags.SynchronizationBehavior                  = RMLPositionFlags::PHASE_SYNCHRONIZATION_IF_POSSIBLE;
    Flags.BehaviorAfterFinalStateOfMotionIsReached = RMLPositionFlags::RECOMPUTE_TRAJECTORY;

    // construct Reflexxes objects
    int numberOfDOFs = 2; //X, Y
    RML = new ReflexxesAPI(numberOfDOFs, data.config.dt()); // degrees of freedom (x,y), nominal cycle time
    IP  = new RMLPositionInputParameters(numberOfDOFs);
    OP  = new RMLPositionOutputParameters(numberOfDOFs);

    // set-up the input parameters
    IP->CurrentPositionVector->VecData      [0] = 0.0; // instead of steering from current to target,
    IP->CurrentPositionVector->VecData      [1] = 0.0; // we steer from zero to delta, so we can better configure

    IP->CurrentVelocityVector->VecData      [0] = _currentVelocityRCS.x;
    IP->CurrentVelocityVector->VecData      [1] = _currentVelocityRCS.y;

    IP->CurrentAccelerationVector->VecData  [0] = 0.0; // not relevant, due to limitation of TypeII library
    IP->CurrentAccelerationVector->VecData  [1] = 0.0;

    IP->MaxVelocityVector->VecData          [0] = spgLimits.vx;
    IP->MaxVelocityVector->VecData          [1] = spgLimits.vy;

    IP->MaxAccelerationVector->VecData      [0] = spgLimits.ax;
    IP->MaxAccelerationVector->VecData      [1] = spgLimits.ay;

    IP->MaxJerkVector->VecData              [0] = 0.0; // not used in TypeII library
    IP->MaxJerkVector->VecData              [1] = 0.0;

    IP->TargetPositionVector->VecData       [0] = _deltaPositionRCS.x;  // steering from currentPos = 0,
    IP->TargetPositionVector->VecData       [1] = _deltaPositionRCS.y;  // to targetPos = deltaPos

    IP->TargetVelocityVector->VecData       [0] = _targetVelocityRCS.x;
    IP->TargetVelocityVector->VecData       [1] = _targetVelocityRCS.y;

    IP->SelectionVector->VecData            [0] = true;
    IP->SelectionVector->VecData            [1] = true;

    // call the Reflexxes Online Trajectory Generation algorithm
    int r1 = RML->RMLPosition(*IP, OP, Flags);
    if (r1 < 0)
    {
        // error state
        return false;
    }

    // output parameters have been evaluated at first tick (data.config.dt())
    // latency correction: evaluate the trajectory at some offset
    double timeOffset = data.config.dt() + data.config.spg().latencyoffset();
    RML->RMLPositionAtAGivenSampleTime(timeOffset, OP);

    // convert outputs
    resultPosition.x = OP->NewPositionVector->VecData[0];
    resultPosition.y = OP->NewPositionVector->VecData[1];

    resultVelocity.x = OP->NewVelocityVector->VecData[0];
    resultVelocity.y = OP->NewVelocityVector->VecData[1];

    // Diag data
    /*TODO
    data.spgCurrentPosition.x  = IP->CurrentPositionVector->VecData[0];
    data.spgCurrentPosition.y  = IP->CurrentPositionVector->VecData[1];

    data.spgCurrentVelocity.x  = IP->CurrentVelocityVector->VecData[0];
    data.spgCurrentVelocity.y  = IP->CurrentVelocityVector->VecData[1];

    data.spgMaxVelocity.x  = IP->MaxVelocityVector->VecData[0];
    data.spgMaxVelocity.y  = IP->MaxVelocityVector->VecData[1];

    data.spgMaxAcceleration.x  = IP->MaxAccelerationVector->VecData[0];
    data.spgMaxAcceleration.y  = IP->MaxAccelerationVector->VecData[1];

    data.spgTargetPosition.x  = IP->TargetPositionVector->VecData[0];
    data.spgTargetPosition.y  = IP->TargetPositionVector->VecData[1];

    data.spgTargetVelocity.x  = IP->TargetVelocityVector->VecData[0];
    data.spgTargetVelocity.y  = IP->TargetVelocityVector->VecData[1];

    data.spgNewPosition.x  = OP->NewPositionVector->VecData[0];
    data.spgNewPosition.y  = OP->NewPositionVector->VecData[1];

    data.spgNewVelocity.x  = OP->NewVelocityVector->VecData[0];
    data.spgNewVelocity.y  = OP->NewVelocityVector->VecData[1];
    */

    // cleanup
    delete RML;
    delete IP;
    delete OP;

    return true;
}


bool SPGVelocitySetpointController::calculatePosRzNonSynchronized(VelocityControlData& data, SpgLimits const &spgLimits, Position2D& resultPosition, Velocity2D &resultVelocity)
{

    // initialize
    ReflexxesAPI                *RML = NULL;
    RMLPositionInputParameters  *IP = NULL;
    RMLPositionOutputParameters *OP = NULL;
    RMLPositionFlags            Flags;
    Flags.SynchronizationBehavior                  = RMLPositionFlags::NO_SYNCHRONIZATION;
    Flags.BehaviorAfterFinalStateOfMotionIsReached = RMLPositionFlags::RECOMPUTE_TRAJECTORY;

    // construct Reflexxes objects
    int numberOfDOFs = 1; // Rz
    RML = new ReflexxesAPI(numberOfDOFs, data.config.dt()); // degrees of freedom (Rz), nominal cycle time
    IP  = new RMLPositionInputParameters(numberOfDOFs);
    OP  = new RMLPositionOutputParameters(numberOfDOFs);

    // set-up the input parameters
    IP->CurrentPositionVector->VecData      [0] = 0.0; // controlling FCS or RCS
    IP->CurrentVelocityVector->VecData      [0] = _currentVelocityRCS.rz;
    IP->CurrentAccelerationVector->VecData  [0] = 0.0;
    IP->MaxVelocityVector->VecData          [0] = spgLimits.vRz;
    IP->MaxAccelerationVector->VecData      [0] = spgLimits.aRz;
    IP->MaxJerkVector->VecData              [0] = 0.0;
    IP->TargetPositionVector->VecData       [0] = _deltaPositionRCS.rz;
    IP->TargetVelocityVector->VecData       [0] = _targetVelocityRCS.rz;
    IP->SelectionVector->VecData            [0] = true;

    // call the Reflexxes Online Trajectory Generation algorithm
    int r1 = RML->RMLPosition(*IP, OP, Flags);
    if (r1 < 0)
    {
        // error state
        return false;
    }

    // output parameters have been evaluated at first tick (data.config.dt())
    // latency correction: evaluate the trajectory at some offset
    double timeOffset = data.config.dt() + data.config.spg().latencyoffset();
    RML->RMLPositionAtAGivenSampleTime(timeOffset, OP);

    // convert outputs
    resultPosition.rz = OP->NewPositionVector->VecData[0];
    resultVelocity.rz = OP->NewVelocityVector->VecData[0];

    // Diag data
    /*TODO
    data.spgCurrentPosition.Rz = IP->CurrentPositionVector->VecData[0];
    data.spgCurrentVelocity.Rz = IP->CurrentVelocityVector->VecData[0];
    data.spgMaxVelocity.Rz = IP->MaxVelocityVector->VecData[0];
    data.spgMaxAcceleration.Rz = IP->MaxAccelerationVector->VecData[0];
    data.spgTargetPosition.Rz = IP->TargetPositionVector->VecData[0];
    data.spgTargetVelocity.Rz = IP->TargetVelocityVector->VecData[0];
    data.spgNewPosition.Rz = OP->NewPositionVector->VecData[0];
    data.spgNewVelocity.Rz = OP->NewVelocityVector->VecData[0];
    */

    // cleanup
    delete RML;
    delete IP;
    delete OP;

    return true;
}



bool SPGVelocitySetpointController::calculateVelXYRzPhaseSynchronized(VelocityControlData& data, SpgLimits const &spgLimits, Position2D& resultPosition, Velocity2D &resultVelocity)
{

    // initialize
    ReflexxesAPI                *RML = NULL;
    RMLVelocityInputParameters  *IP = NULL;
    RMLVelocityOutputParameters *OP = NULL;
    RMLVelocityFlags            Flags;
    Flags.SynchronizationBehavior = RMLVelocityFlags::PHASE_SYNCHRONIZATION_IF_POSSIBLE;

    // construct Reflexxes objects
    int numberOfDOFs = 3; //X, Y, Rz
    RML = new ReflexxesAPI(numberOfDOFs, data.config.dt()); // degrees of freedom (x,y,Rz), nominal cycle time
    IP  = new RMLVelocityInputParameters(numberOfDOFs);
    OP = new RMLVelocityOutputParameters(numberOfDOFs);

    // set-up the input parameters
    IP->CurrentPositionVector->VecData      [0] = 0.0; // instead of steering from current to target,
    IP->CurrentPositionVector->VecData      [1] = 0.0; // we steer from zero to delta, so we can better configure
    IP->CurrentPositionVector->VecData      [2] = 0.0; // controlling FCS or RCS

    IP->CurrentVelocityVector->VecData      [0] = _currentVelocityRCS.x;
    IP->CurrentVelocityVector->VecData      [1] = _currentVelocityRCS.y;
    IP->CurrentVelocityVector->VecData      [2] = _currentVelocityRCS.rz;

    IP->CurrentAccelerationVector->VecData  [0] = 0.0; // not relevant, due to limitation of TypeII library
    IP->CurrentAccelerationVector->VecData  [1] = 0.0;
    IP->CurrentAccelerationVector->VecData  [2] = 0.0;

    IP->MaxAccelerationVector->VecData      [0] = spgLimits.ax;
    IP->MaxAccelerationVector->VecData      [1] = spgLimits.ay;
    IP->MaxAccelerationVector->VecData      [2] = spgLimits.aRz;

    IP->MaxJerkVector->VecData              [0] = 0.0; // not used in TypeII library
    IP->MaxJerkVector->VecData              [1] = 0.0;
    IP->MaxJerkVector->VecData              [2] = 0.0;

    IP->TargetVelocityVector->VecData       [0] = std::clamp(_targetVelocityRCS.x,   (double) -spgLimits.vx,  (double) spgLimits.vx);
    IP->TargetVelocityVector->VecData       [1] = std::clamp(_targetVelocityRCS.y,   (double) -spgLimits.vy,  (double) spgLimits.vy);
    IP->TargetVelocityVector->VecData       [2] = std::clamp(_targetVelocityRCS.rz,  (double) -spgLimits.vRz, (double) spgLimits.vRz);

    IP->SelectionVector->VecData            [0] = true;
    IP->SelectionVector->VecData            [1] = true;
    IP->SelectionVector->VecData            [2] = true;

    // call the Reflexxes Online Trajectory Generation algorithm
    int r1 = RML->RMLVelocity(*IP, OP, Flags);
    if (r1 < 0)
    {
        // error state
        return false;
    }
    else if (r1 == ReflexxesAPI::RMLResultValue::RML_FINAL_STATE_REACHED)
    {
        // final state reached: current velocity == target velocity
        resultVelocity.x   = _targetVelocityRCS.x;
        resultVelocity.y   = _targetVelocityRCS.y;
        resultVelocity.rz  = _targetVelocityRCS.rz;
    }
    else
    {
        // output parameters have been evaluated at first tick (data.config.dt())
        // latency correction: evaluate the trajectory at some offset
        double timeOffset = data.config.dt() + data.config.spg().latencyoffset();
        int r2 = RML->RMLVelocityAtAGivenSampleTime(timeOffset, OP);
        if (r2 < 0)
        {
            // error state
            return false;
        }
        else if (r2 == ReflexxesAPI::RMLResultValue::RML_FINAL_STATE_REACHED)
        {
            // final state reached: current velocity == target velocity
            resultVelocity.x   = _targetVelocityRCS.x;
            resultVelocity.y   = _targetVelocityRCS.y;
            resultVelocity.rz  = _targetVelocityRCS.rz;
        }
        else
        {
            if (std::isnan(OP->NewVelocityVector->VecData[0])
                || std::isnan(OP->NewVelocityVector->VecData[1])
                || std::isnan(OP->NewVelocityVector->VecData[2]))
            {
                // unknown why results are nan
                return false;
            }

            resultVelocity.x = OP->NewVelocityVector->VecData[0];
            resultVelocity.y = OP->NewVelocityVector->VecData[1];
            resultVelocity.rz = OP->NewVelocityVector->VecData[2];
        }
    }
    
    // Diag data
    /*TODO
    data.spgCurrentPosition.x  = IP->CurrentPositionVector->VecData[0];
    data.spgCurrentPosition.y  = IP->CurrentPositionVector->VecData[1];
    data.spgCurrentPosition.Rz = IP->CurrentPositionVector->VecData[2];

    data.spgCurrentVelocity.x  = IP->CurrentVelocityVector->VecData[0];
    data.spgCurrentVelocity.y  = IP->CurrentVelocityVector->VecData[1];
    data.spgCurrentVelocity.Rz = IP->CurrentVelocityVector->VecData[2];

    data.spgMaxAcceleration.x  = IP->MaxAccelerationVector->VecData[0];
    data.spgMaxAcceleration.y  = IP->MaxAccelerationVector->VecData[1];
    data.spgMaxAcceleration.Rz = IP->MaxAccelerationVector->VecData[2];

    data.spgTargetVelocity.x  = IP->TargetVelocityVector->VecData[0];
    data.spgTargetVelocity.y  = IP->TargetVelocityVector->VecData[1];
    data.spgTargetVelocity.Rz = IP->TargetVelocityVector->VecData[2];

    data.spgNewPosition.x  = OP->NewPositionVector->VecData[0];
    data.spgNewPosition.y  = OP->NewPositionVector->VecData[1];
    data.spgNewPosition.Rz = OP->NewPositionVector->VecData[2];

    data.spgNewVelocity.x  = OP->NewVelocityVector->VecData[0];
    data.spgNewVelocity.y  = OP->NewVelocityVector->VecData[1];
    data.spgNewVelocity.Rz = OP->NewVelocityVector->VecData[2];
    */

    // cleanup
    delete RML;
    delete IP;
    delete OP;

    return true;
}

