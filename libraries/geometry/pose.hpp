#ifndef _MRA_LIBRARIES_GEOMETRY_POSE_HPP
#define _MRA_LIBRARIES_GEOMETRY_POSE_HPP

#include "datatypes/Pose.pb.h"

// MRA::Geometry::Pose is a class that adds operations to protobuf MRA::Datatypes::Pose

namespace MRA::Geometry
{

class Pose
{
public:
    // direct read/write to the data members
    double x;
    double y;
    double z;
    double rx;
    double ry;
    double rz;

    // constructors, destructor
    Pose(double x_=0.0, double y_=0.0, double z_=0.0, double rx_=0.0, double ry_=0.0, double rz_=0.0);
    Pose(const MRA::Datatypes::Pose&);
    ~Pose();

    // basic operations
    double size() const;

    // arithmetic operators
    Pose operator+(const Pose& other) const;
    Pose& operator+=(const Pose& other);
    Pose operator*(double f) const;

}; // class Pose

// clip rotation/angle to [0, 2pi) according to MSL coordinate system specification
double clip_rot(double rot);

} // namespace MRA::geometry

#endif // #ifndef _MRA_LIBRARIES_GEOMETRY_POSE_HPP
