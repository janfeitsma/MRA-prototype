#include "angles.hpp"
#include <cmath>

// wrap rotation/angle to [0, 2pi) for standardized coordinates
double MRA::Geometry::wrap_2pi(double rot)
{
    double b = 2 * M_PI;
    double result = fmod(rot, b);
    if (result < 0) result += b; // ensure positive
    return result;
}

// wrap rotation/angle to [-pi, pi) for subsequent abs()-and-compare
double MRA::Geometry::wrap_pi(double rot)
{
    double result = MRA::Geometry::wrap_2pi(rot);
    if (result >= M_PI) result -= 2 * M_PI;
    return result;
}

// relative angle in MSL-FCS, where 0 is facing opponent goal
double MRA::Geometry::calc_facing_angle_fcs(double from_x, double from_y, double to_x, double to_y)
{
    return MRA::Geometry::wrap_pi(atan2(to_y - from_y, to_x - from_x) - 0.5 * M_PI);
}

