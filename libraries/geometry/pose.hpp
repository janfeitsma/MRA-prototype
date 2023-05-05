#ifndef _MRA_LIBRARIES_GEOMETRY_POSE_HPP
#define _MRA_LIBRARIES_GEOMETRY_POSE_HPP

#include "datatypes/Pose.pb.h"

// MRA::Geometry::Pose is a base class that adds operations to protobuf MRA::Datatypes::Pose
// MRA::Geometry::Velocity is special version of Pose that adds coordinate transformations
// MRA::Geometry::Position is special version of Pose that adds coordinate transformations, 
//    ensuring angles are wrapped to [-pi,pi)

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
    double size() const; // sqrt(x^2+y^2+z^2)

    // arithmetic operators
    virtual Pose operator+(const Pose& other) const;
    virtual Pose& operator+=(const Pose& other);
    virtual Pose operator*(double f) const;
    virtual Pose& operator*=(double f);

    // functions call wrap_angles, left to be implemented by Position
    virtual void wrap_angles();

}; // class Pose

} // namespace MRA::Geometry

#endif // #ifndef _MRA_LIBRARIES_GEOMETRY_POSE_HPP

