/*
 * VelocityControl.cpp
 *
 *  Created on: May 2023
 *      Author: Jan Feitsma
 */

// own package
#include "VelocityControl.hpp"

/* TODO
#include "VelocitySetpointControllers.hpp"
#include "VelocityControlAlgorithms.hpp"
*/

VelocityControl::VelocityControl()
{
}

VelocityControl::~VelocityControl()
{
}

void VelocityControl::iterate()
{
    // this function assumes all required inputs (Inputs, Params, State) are set under "data"

    // when was the last time VelocityControl was poked?
    double age = 2.0; //data.timestamp - data.state.timestamp(); // TODO: MRA::libraries::time (for Timestamp and Duration conversions, operations)
    if (age > data.config.timeout())
    {
        // last tick was too long ago, probably robot has been standing still for a while
        // -> reset state.velocity in order to properly limit acceleration
        data.state.mutable_velocityrcs()->Clear();
        data.state.mutable_velocitysetpointfcs()->Clear();
    }
}

/* TODO below
    // configure the sequence of algorithms
    std::vector<VelocityControlAlgorithm *> algorithms; // previously known as 'blocks'

    // prepare for velocity control to sub-target
    algorithms.push_back(new CalculateDeltas());
    algorithms.push_back(new ShiftBallOffset());

    // velocity control (in FCS or RCS, depends on configuration)
    // roadmap: this part is probably going to be moved to lower-level motion control
    // new motion platform might implement this using a setpoint generator on lower level (?),
    // but for simulation we will always need this, so then it would have to become switchable
    algorithms.push_back(new SelectVelocityController());
    algorithms.push_back(new CalculateVelocity(boost::bind(&VelocityControl::setupAndGetVelocitySetpointController, this))); // just-in-time callback
    // limiters and everything else below is performed in RCS
    //algorithms.push)_back(new CalculateAccelerating());
    //algorithms.push_back(new ApplyLimits());
    //algorithms.push_back(new CalculateDeltas()); // needed for deadzone in case ball shift was applied
    algorithms.push_back(new Deadzone());


    // Apply TokyoDrift after Deadzone -- otherwise Deadzone might set vx=0 while attempting to TokyoDrift
    // Alternatively, DeadZone should look at ResultVelocityRCS next to DeltaPositionRCS
    algorithms.push_back(new UnShiftBallOffset());
    //algorithms.push_back(new ApplyTokyoDrift());

    // execute the sequence of algorithms
    for (auto it = algorithms.begin(); it != algorithms.end(); ++it)
    {
        if (!data.done)
        {
            (*it)->execute(data);
        }
    }
}

void VelocityControl::getInputs()
{
    // configuration is handled at construction and upon change
    if (_inputInterface != NULL)
    {
        // query RTDB once so we could do repeated gets
        _inputInterface->fetch();

        // get and store data
        robotPosVel sp = _inputInterface->getRobotPosVelSetpoint();

        data.robotPosVelMoveType = sp.robotPosVelType;
        switch (sp.robotPosVelType)
        {
            case robotPosVelEnum::POS_ONLY:
            {
                data.target.pos = sp.position;
                data.target.vel = pose();
                break;
            }
            case robotPosVelEnum::VEL_ONLY:
            {
                data.target.pos = pose();
                data.target.vel = sp.velocity;
                break;
            }
            case robotPosVelEnum::POSVEL:
            {
                data.target.pos = sp.position;
                data.target.vel = sp.velocity;
                break;
            }
            default:
            {
                break;
            }
        }

        data.motionType = sp.motionType;

        data.robot = _inputInterface->getRobotState();
        data.teamMembers = _inputInterface->getTeamMembers();
        data.balls = _inputInterface->getBalls();
    }
}

void VelocityControl::setOutputs()
{
    if (_outputInterface != NULL)
    {
        robotVelocity sp;
        sp.x = data.resultVelocityRcs.x;
        sp.y = data.resultVelocityRcs.y;
        sp.Rz = data.resultVelocityRcs.phi;
        _outputInterface->setVelocity(sp);
    }
}

void VelocityControl::prepare()
{
    data.reset();

    // new configuration?
    if (_vcConfigInterface != NULL)
    {
        bool config_changed = _vcConfigInterface->get(data.vcConfig);
        if (config_changed)
        {
            // cleanup
            clearVelocitySetpointController();
        }
    }
    if (_ppConfigInterface != NULL)
    {
        _ppConfigInterface->get(data.ppConfig);
    }
    if (_exConfigInterface != NULL)
    {
        _exConfigInterface->get(data.exConfig);
    }

    data.vcSetpointConfig.type = VelocitySetpointControllerTypeEnum::NONE;

    // timestepping
    if (data.exConfig.frequency > 0)
    {
        data.dt = 1.0 / data.exConfig.frequency;
    }
    else
    {
        data.dt = 1.0 / 20;
    }
}

void VelocityControl::clearVelocitySetpointController()
{
    if (_velocitySetpointController != NULL)
    {
        delete _velocitySetpointController;
    }
    _velocitySetpointController = NULL;
    _currentVelocitySetpointControllerType = VelocitySetpointControllerTypeEnum::NONE;
}

void VelocityControl::setupVelocitySetpointController()
{
    bool needReInit = (_velocitySetpointController == NULL) ||
        (data.vcSetpointConfig.type != _currentVelocitySetpointControllerType);
    // perform the re-init?
    if (needReInit)
    {
        clearVelocitySetpointController();
        switch (data.vcSetpointConfig.type)
        {
            case VelocitySetpointControllerTypeEnum::NONE:
                _velocitySetpointController = new StopVelocitySetpointController();
                break;
            case VelocitySetpointControllerTypeEnum::LINEAR:
                _velocitySetpointController = new LinearVelocitySetpointController();
                break;
            case VelocitySetpointControllerTypeEnum::PID:
                _velocitySetpointController = new PIDVelocitySetpointController(data.currentMotionTypeConfig.pid);
                break;
            case VelocitySetpointControllerTypeEnum::SPG:
                if (data.vcSetpointConfig.coordinateSystem == CoordinateSystemEnum::FCS)
                {
                }
                _velocitySetpointController = new SPGVelocitySetpointController(data.currentMotionTypeConfig.setPointGenerator);
                break;
            default:
        }
        _currentVelocitySetpointControllerType = data.vcSetpointConfig.type;
    }
}

AbstractVelocitySetpointController *VelocityControl::getVelocitySetpointController()
{
    return _velocitySetpointController;
}

AbstractVelocitySetpointController *VelocityControl::setupAndGetVelocitySetpointController()
{
    // this function should be called just-in-time, from CalculateVelocity
    setupVelocitySetpointController();
    return getVelocitySetpointController();
}

*/

