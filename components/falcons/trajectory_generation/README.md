# Summary

TrajectoryGeneration is a simulation component around VelocityControl.

It is not used on Falcons robots. It is intended as a testsuite / mini-simulation / plotting component.

(It might also be used for basic tuning.)

# Scope and context

This component is responsible for generating a trajectory all the way from initial pos/vel to target setpoint pos/vel.
The resulting trajectory is a sequence of samples that can be processed further.

It could be used in robot software for estimating move duration.

There is a little simulation model of how a robot velocity setpoint in RCS affects the robot (worldstate).

The quality of this simulation is currently limited. One could make it more advanced (add inertia, latency, ...?).

# Interface details

See [Input.proto](interface/Input.proto) (same as VelocityControl) and [Output.proto](interface/Output.proto).

Configuration parameters are basically empty, defaults from VelocityControl are taken, with optional overrules.

# Design

The implementation is basically an iteration which repeatedly calls VelocityControl until the move is done.

# Examples

See `trajectory.py -h`.

```
    # generate a very simple x-only trajectory, print a brief report
    $ ./trajectory.py --tx 0.1 brief
    input:
        target setpoint position x  :    0.1000
    parameters overrule:
        n/a
    result:
        duration                    :    0.4250
        number of samples           :   17
        final position x            :    0.0747
        final velocity vx           :    0.2231
    # note how the final sample stops abruptly before reaching target
    # this is because default VelocityControl tolerance is 3cm,
    # so the iteration stops after 7cm of requested 10cm

    # overrule tolerance parameter to become much tighter
    $ ./trajectory.py --tx=0.1 -p '{"deadzone": {"toleranceXY": 0.001}}'
    input:
        target setpoint position x  :    0.1000
    parameters overrule:
        {"deadzone": {"toleranceXY": 0.001}}
    result:
        duration                    :    0.6500
        number of samples           :   26
        final position x            :    0.0991
        final velocity vx           :    0.0086

    # rotate, cross the angle wrap-around at pi, show values in a rz-only table
    $ ./trajectory.py --rz=3.13 --trz=-3.13 --dofs rz table
         iter         t    pos.rz    vel.rz
    =======================================
            1    0.0250    3.1309    0.0375
            2    0.0500    3.1328    0.0750
            3    0.0750    3.1356    0.1125
            4    0.1000    3.1394    0.1500
            5    0.1250   -3.1391    0.1875
            6    0.1500   -3.1335    0.2250
```

The following plots compare the effect of the `synchronizeRotation` configuration parameter of VelocityControl.

## Non-synchronized rotation

Visual example of default configuration (greedy rotation):

![non-synchronized rotation](tst/non-sync-rot.png)

Command used: `./trajectory.py --x=-2 --y=-3 --rz=1 --tx=3 --ty=2 --trz=-3 plot`

## Synchronized rotation

![synchronized rotation](tst/sync-rot.png)

Command used: `./trajectory.py --x=-2 --y=-3 --rz=1 --tx=3 --ty=2 --trz=-3 plot -p '{"spg": {"synchronizeRotation": true}}'`

