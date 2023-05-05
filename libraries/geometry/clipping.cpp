#include "clipping.hpp"
#include <cmath>

// clip rotation/angle to [0, 2pi) for standardized coordinates
double MRA::Geometry::clip_2pi(double rot)
{
    double b = 2 * M_PI;
    double result = fmod(rot, b);
    if (result < 0) result += b; // ensure positive
    return result;
}

// clip rotation/angle to [-pi, pi) for subsequent abs()-and-compare
double MRA::Geometry::clip_pi(double rot)
{
    double result = MRA::Geometry::clip_2pi(rot);
    if (result >= M_PI) result -= 2 * M_PI;
    return result;
}

