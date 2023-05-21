#ifndef _MRA_LIBRARIES_GEOMETRY_VELOCITY_HPP
#define _MRA_LIBRARIES_GEOMETRY_VELOCITY_HPP

#include "pose.hpp"

namespace MRA::Geometry
{

// MRA::Geometry::Velocity is special version of Pose that adds coordinate transformations

class Velocity: public Pose
{
public:
    Velocity(double x_=0.0, double y_=0.0, double z_=0.0, double rx_=0.0, double ry_=0.0, double rz_=0.0);
    Velocity(MRA::Geometry::Pose const &p);
    Velocity(MRA::Datatypes::Pose const &p);

    // transform (planar) between RCS and FCS using refpos in FCS
    Velocity& transformRcsToFcs(Velocity const &refpos);
    Velocity& transformFcsToRcs(Velocity const &refpos);

}; // class Velocity: public Pose

} // namespace MRA::geometry

#endif // #ifndef _MRA_LIBRARIES_GEOMETRY_VELOCITY_HPP

