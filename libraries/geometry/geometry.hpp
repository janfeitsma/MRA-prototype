#ifndef _MRA_LIBRARIES_GEOMETRY_GEOMETRY_HPP
#define _MRA_LIBRARIES_GEOMETRY_GEOMETRY_HPP

// MRA::Geometry::Pose is a class that adds operations to protobuf MRA::Datatypes::Pose
#include "libraries/geometry/pose.hpp"

namespace MRA::Geometry
{

// calculate angle between two positions
double calc_rz_between(MRA::Geometry::Pose const &from, MRA::Geometry::Pose const &to);

} // namespace MRA::Geometry

#endif // #ifndef _MRA_LIBRARIES_GEOMETRY_GEOMETRY_HPP

