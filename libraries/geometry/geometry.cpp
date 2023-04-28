#include "geometry.hpp"
#include <cmath>

// vectorsize: sqrt(x*x + y*y + z*z)
double MRA::geometry::vectorsize(Pose const &p)
{
    return sqrt(p.x() * p.x() + p.y() * p.y() + p.z() * p.z());
}

// relative angle
double MRA::geometry::calc_rz_between(Pose const &from, Pose const &to)
{
    return clip_rot(atan2(to.y() - from.y(), to.x() - from.x()));
}

// clip rotation/angle to [0, 2pi) according to MSL coordinate system specification
double clip_rot(double rot)
{
    double b = 2 * M_PI;
    double result = fmod(rot, b);
    if (result < 0) result += b; // ensure positive
    return result;
}
