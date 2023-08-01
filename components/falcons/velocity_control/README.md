# Summary

VelocityControl is responsible for smooth controlled movement of the robot.

It uses [Reflexxes Type II](../../../libraries/reflexxes/README.md) trajectory setpoint generator which limits velocity and acceleration.

TODO: a nice picture (KSTplot?) of combined X,Y,Rz trajectories, sync/nonsync.

# Scope and context

This component is responsible for applying vel/acc limits and not losing ball while driving. It uses [Reflexxes Type II](../../../libraries/reflexxes/README.md) trajectory setpoint generator.

This component is NOT responsible for obstacle avoidance. That is done by PathPlanning just before VelocityControl, PathPlanning converts a desired ("unsafe") target setpoint (FCS) to a "safe" intermediate target setpoint (FCS), avoiding obstacles.

The output robot velocity setpoint (in RCS) is given to VelocityTransform, to be transformed to motor setpoints.

# Interface details

See [Input.proto](interface/Input.proto) and [Output.proto](interface/Output.proto).

There are quite some configuration parameters, see [Params.proto](interface/Params.proto) and [DefaultParams.json](interface/DefaultParams.json).

At least one (default) motion profile needs to be configured. When another motion profile with name `withBall` is configured, then it will be used to overrule limits while dribbling.

# Design

A sequence of sub-algorithms is applied. They are somewhat configurable.

# History

Original Falcons source: https://github.com/Falcons-Robocup/code/tree/master/packages/velocityControl/src.

PID controllers and configuration have been removed, in favour of Reflexxes.
 
