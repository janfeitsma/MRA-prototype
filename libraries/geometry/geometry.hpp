#ifndef _MRA_LIBRARIES_GEOMETRY_GEOMETRY_HPP
#define _MRA_LIBRARIES_GEOMETRY_GEOMETRY_HPP

#include "datatypes/Pose.pb.h"

namespace MRA::geometry
{

// vectorsize: sqrt(x*x + y*y + z*z)
double vectorsize(Pose const &p);

// convert Pose to XY
//XY pose_to_xy(Pose const &p);

// convert XY to Pose
//Pose xy_to_pose(XY const &xy);

// relative angle
double calc_rz_between(Pose const &from, Pose const &to);

} // namespace MRA::geometry

// clip rotation/angle to [0, 2pi) according to MSL coordinate system specification
double clip_rot(double rot);

// add two Pose objects
inline Pose operator+(Pose const &lhs, Pose const &rhs)
{
    Pose result;
    result.set_x(lhs.x() + rhs.x());
    result.set_y(lhs.y() + rhs.y());
    result.set_z(lhs.z() + rhs.z());
    result.set_rx(clip_rot(lhs.rx() + rhs.rx()));
    result.set_ry(clip_rot(lhs.ry() + rhs.ry()));
    result.set_rz(clip_rot(lhs.rz() + rhs.rz()));
    return result;
}

// multiply a Pose object with a scalar
inline Pose operator*(Pose const &p, double f)
{
    Pose result;
    result.set_x(p.x() * f);
    result.set_y(p.y() * f);
    result.set_z(p.z() * f);
    result.set_rx(clip_rot(p.rx() * f));
    result.set_ry(clip_rot(p.ry() * f));
    result.set_rz(clip_rot(p.rz() * f));
    return result;
}

#endif // #ifndef _MRA_LIBRARIES_GEOMETRY_GEOMETRY_HPP

