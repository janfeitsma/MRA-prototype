# Summary

VelocityControl is responsible for smooth controlled movement of the robot.


# Scope and context

This component is responsible for applying vel/acc limits and not losing ball while driving. .

This component is NOT responsible for obstacle avoidance. That is done by other components before VelocityControl.

The output robot velocity setpoint (in RCS) is given to VelocityTransform, to be transformed to motor setpoints.

# Interface details

See [Input.proto](interface/Input.proto) and [Output.proto](interface/Output.proto).

There are quite some configuration parameters, see [Params.proto](interface/Params.proto) and [DefaultParams.json](interface/DefaultParams.json).

At least one (default) motion profile needs to be configured. When another motion profile with name `withBall` is configured, then it will be used to overrule limits while dribbling.

# Design

A sequence of sub-algorithms is applied. They are somewhat configurable.

# History

Original RobotSports source: src/mod/tactics/skills/MoveToBall.cpp (last part of action)

 
