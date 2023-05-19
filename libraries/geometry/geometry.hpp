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
#include "libraries/geometry/clipping.hpp" // TODO rename to angles.hpp and add calc_facing_angle_fcs (lost during merge)

#endif // #ifndef _MRA_LIBRARIES_GEOMETRY_GEOMETRY_HPP

