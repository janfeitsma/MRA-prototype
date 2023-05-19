#include "geometry.hpp"
#include <cmath>

// vectorsize: sqrt(x*x + y*y + z*z)
double MRA::geometry::vectorsize(MRA::Datatypes::Pose const &p)
{
    return sqrt(p.x() * p.x() + p.y() * p.y() + p.z() * p.z());
}

// relative angle
double MRA::geometry::calc_facing_angle_fcs(MRA::Datatypes::Pose const &from, MRA::Datatypes::Pose const &to)
{
    return clip_rot(atan2(to.y() - from.y(), to.x() - from.x()) - 0.5 * M_PI);
}

// clip rotation/angle to [0, 2pi) according to MSL coordinate system specification
double clip_rot(double rot)
{
    double b = 2 * M_PI;
    double result = fmod(rot, b);
    if (result < 0) result += b; // ensure positive
    return result;
}
