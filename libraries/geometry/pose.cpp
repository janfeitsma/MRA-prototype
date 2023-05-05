#include "pose.hpp"
#include <cmath>

using namespace MRA::Geometry;

// constructors, destructor
Pose::Pose(double x_, double y_, double z_, double rx_, double ry_, double rz_)
{
    x = x_;
    y = y_;
    z = z_;
    rx = rx_;
    ry = ry_;
    rz = rz_;
    clip();
}

Pose::Pose(const MRA::Datatypes::Pose& p)
{
    x = p.x();
    y = p.y();
    z = p.z();
    rx = p.rx();
    ry = p.ry();
    rz = p.rz();
    clip();
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
    rx += other.rx;
    ry += other.ry;
    rz += other.rz;
    clip();
    return *this;
}

Pose Pose::operator*(double f) const
{
    return Pose(x * f, y * f, z * f, rx * f, ry * f, rz * f);
}

