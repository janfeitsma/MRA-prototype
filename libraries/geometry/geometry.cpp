#include "geometry.hpp"
#include <cmath>

// calculate angle between two positions
double MRA::Geometry::calc_rz_between(MRA::Geometry::Pose const &from, MRA::Geometry::Pose const &to)
{
    return MRA::Geometry::clip_rot(atan2(to.y - from.y, to.x - from.x));
}
