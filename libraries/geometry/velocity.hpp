#ifndef _MRA_LIBRARIES_GEOMETRY_VELOCITY_HPP
#define _MRA_LIBRARIES_GEOMETRY_VELOCITY_HPP

#include "pose.hpp"

namespace MRA::Geometry
{

// MRA::Geometry::Velocity is special version of Pose that adds coordinate transformations

class Velocity: public Pose
{
public:
    Velocity() : Pose() {};
    Velocity(const Pose& p) : Pose(p) {};

}; // class Velocity: public Pose

} // namespace MRA::geometry

#endif // #ifndef _MRA_LIBRARIES_GEOMETRY_VELOCITY_HPP

