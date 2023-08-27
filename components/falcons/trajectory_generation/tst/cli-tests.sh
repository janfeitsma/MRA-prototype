#!/bin/bash

BASE=components/falcons/trajectory_generation/tst

EXE="$BASE/trajectory.py --exe $1"

set -e

# test case: can produce a help text
$EXE -h > /dev/null


###
### NOTE: below tests are currently sensitive to changes in both implementation and tuning of component VelocityControl
###


# test case: help text example 1
# generate a very simple x-only trajectory, print a brief report
$EXE --tx 0.1 brief | diff - $BASE/output_example1.txt

# test case: help text example 2
# overrule tolerance parameter to become much tighter
$EXE --tx=0.1 -p '{"deadzone": {"toleranceXY": 0.001}}' | diff - $BASE/output_example2.txt

# test case: help text example 3
# rotate, cross the angle wrap-around at pi, show values in a rz-only table
$EXE --rz=3.13 --trz=-3.13 --dofs rz table  | diff - $BASE/output_example3.txt

# test case: move with all dofs at once
$EXE --x=-2 --y=-3 --rz=1 --tx=3 --ty=2 --trz=-3 | diff - $BASE/output_alldofs.txt

# test case: produce json
$EXE --x=-2 json | diff - $BASE/output_json.txt

