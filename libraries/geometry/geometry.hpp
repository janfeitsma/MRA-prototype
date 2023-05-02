#ifndef _MRA_LIBRARIES_GEOMETRY_GEOMETRY_HPP
#define _MRA_LIBRARIES_GEOMETRY_GEOMETRY_HPP

#include "datatypes/Pose.pb.h"

namespace MRA::geometry
{

// vectorsize: sqrt(x*x + y*y + z*z)
double vectorsize(MRA::Datatypes::Pose const &p);

// relative angle
double calc_rz_between(MRA::Datatypes::Pose const &from, MRA::Datatypes::Pose const &to);

} // namespace MRA::geometry

// clip rotation/angle to [0, 2pi) according to MSL coordinate system specification
double clip_rot(double rot);

// add two Pose objects
inline MRA::Datatypes::Pose operator+(MRA::Datatypes::Pose const &lhs, MRA::Datatypes::Pose const &rhs)
{
    MRA::Datatypes::Pose result;
    result.set_x(lhs.x() + rhs.x());
    result.set_y(lhs.y() + rhs.y());
    result.set_z(lhs.z() + rhs.z());
    result.set_rx(clip_rot(lhs.rx() + rhs.rx()));
    result.set_ry(clip_rot(lhs.ry() + rhs.ry()));
    result.set_rz(clip_rot(lhs.rz() + rhs.rz()));
    return result;
}

// multiply a Pose object with a scalar
inline MRA::Datatypes::Pose operator*(MRA::Datatypes::Pose const &p, double f)
{
    MRA::Datatypes::Pose result;
    result.set_x(p.x() * f);
    result.set_y(p.y() * f);
    result.set_z(p.z() * f);
    result.set_rx(clip_rot(p.rx() * f));
    result.set_ry(clip_rot(p.ry() * f));
    result.set_rz(clip_rot(p.rz() * f));
    return result;
}

#endif // #ifndef _MRA_LIBRARIES_GEOMETRY_GEOMETRY_HPP

