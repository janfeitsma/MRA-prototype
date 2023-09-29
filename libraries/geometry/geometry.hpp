#ifndef _MRA_LIBRARIES_GEOMETRY_GEOMETRY_HPP
#define _MRA_LIBRARIES_GEOMETRY_GEOMETRY_HPP

// MRA::Geometry::Pose is a base class that adds operations to protobuf MRA::Datatypes::Pose
// MRA::Geometry::Velocity is special version of Pose that adds coordinate transformations
// MRA::Geometry::Position is special version of Pose that adds coordinate transformations,
//    ensuring angles are wrapped to [-pi,pi)
#include "libraries/geometry/pose.hpp"
#include "libraries/geometry/position.hpp"
#include "libraries/geometry/velocity.hpp"

// angle wrapping & clipping operations
#include "libraries/geometry/angles.hpp"

// MRA::Geometry::Point is a class that adds operations to protobuf MRA::Datatypes::Point (xy only)
#include "libraries/geometry/point.hpp"

#endif // #ifndef _MRA_LIBRARIES_GEOMETRY_GEOMETRY_HPP

