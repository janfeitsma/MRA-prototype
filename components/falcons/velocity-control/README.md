# Summary

VelocityControl is responsible for smooth controlled movement of the robot.

It uses [libraries/reflexxes/README.md](Reflexxes Type II) trajectory setpoint generator which limits velocity and acceleration.

# Context

The input robot target setpoint (in FCS) is calculated by PathPlanning, which is responsible for obstacle avoidance.

The output robot velocity setpoint (in RCS) is given to VelocityTransform, to be transformed to motor setpoints.

# Interface details

See [interface/Inputs.proto](Inputs.proto) and [interface/Outputs.proto](Outputs.proto).

There are quite some configuration parameters, see [interface/Params.proto](Params.proto) and DefaultParams.

# Design

A sequence of sub-algorithms is applied. They are somewhat configurable.

# History

PID controllers and configuration have been removed, in favour or Reflexxes.

