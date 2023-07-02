#ifndef _MRA_LIBRARIES_GEOMETRY_POSE_HPP
#define _MRA_LIBRARIES_GEOMETRY_POSE_HPP

#include "datatypes/Pose.pb.h"
#include "point.hpp"

// MRA::Geometry::Pose is a base class that adds operations to protobuf MRA::Datatypes::Pose
// MRA::Geometry::Velocity is special version of Pose that adds coordinate transformations
// MRA::Geometry::Position is special version of Pose that adds coordinate transformations, 
//    ensuring angles are wrapped to [-pi,pi)
//    NOTE: this deviates from the 2014 standard of wrapping rz to [0,2pi), for the following reasons:
//    1. practical, ease of coding (for instance abs-and-compare)
//    2. better fits with generalized 6dof approach

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
    Pose(MRA::Datatypes::Pose const &p);
    ~Pose();

    // basic operations
    void reset();
    double size() const; // sqrt(x^2+y^2+z^2)
    std::string xyrz_str() const; // string conversion
    bool is_zero(double tolerance = 0.0) const; // check all 6 dof

    // arithmetic operators
    virtual Pose operator+(Pose const&other) const;
    virtual Pose& operator+=(Pose const&other);
    virtual Pose operator-(Pose const&other) const;
    virtual Pose& operator-=(Pose const&other);
    virtual Pose operator*(double f) const;
    virtual Pose& operator*=(double f);
    virtual Pose operator/(double f) const;
    virtual Pose& operator/=(double f);

    // other operators
    operator MRA::Geometry::Point() const;
    operator MRA::Datatypes::Point() const;
    operator MRA::Datatypes::Pose() const;

    // functions call wrap_angles, left to be implemented by Position
    virtual void wrap_angles();

}; // class Pose

} // namespace MRA::Geometry

#endif // #ifndef _MRA_LIBRARIES_GEOMETRY_POSE_HPP

