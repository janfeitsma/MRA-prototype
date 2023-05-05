#include "pose.hpp"
#include <cmath>

using namespace MRA::Geometry;

// clip rotation/angle to [0, 2pi) according to MSL coordinate system specification
double MRA::Geometry::clip_rot(double rot)
{
    double b = 2 * M_PI;
    double result = fmod(rot, b);
    if (result < 0) result += b; // ensure positive
    return result;
}

// constructors, destructor
Pose::Pose(double x_, double y_, double z_, double rx_, double ry_, double rz_)
{
    x = x_;
    y = y_;
    z = z_;
    rx = clip_rot(rx_);
    ry = clip_rot(ry_);
    rz = clip_rot(rz_);
}

Pose::Pose(const MRA::Datatypes::Pose& p)
{
    x = p.x();
    y = p.y();
    z = p.z();
    rx = clip_rot(p.rx());
    ry = clip_rot(p.ry());
    rz = clip_rot(p.rz());
}

Pose::~Pose()
{
}

// basic operations
double Pose::size() const
{
    return sqrt(x * x + y * y + z * z);
}    

Pose Pose::operator+(const Pose& other) const
{
    return Pose(x + other.x, y + other.y, z + other.z, rx + other.rx, ry + other.ry, rz + other.rz);
}

Pose& Pose::operator+=(const Pose& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    rx = clip_rot(rx + other.rx);
    ry = clip_rot(ry + other.ry);
    rz = clip_rot(rz + other.rz);
    return *this;
}

Pose Pose::operator*(double f) const
{
    return Pose(x * f, y * f, z * f, rx * f, ry * f, rz * f);
}

/*
// vectorsize: sqrt(x*x + y*y + z*z)
double MRA::geometry::vectorsize(MRA::Datatypes::Pose const &p)
{
}

// relative angle
double MRA::geometry::calc_rz_between(MRA::Datatypes::Pose const &from, MRA::Datatypes::Pose const &to)
{
    return clip_rot(atan2(to.y() - from.y(), to.x() - from.x()));
}

*/
