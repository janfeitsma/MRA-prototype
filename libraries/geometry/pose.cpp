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
    wrap_angles();
}

Pose::Pose(const MRA::Datatypes::Pose& p)
{
    x = p.x();
    y = p.y();
    z = p.z();
    rx = p.rx();
    ry = p.ry();
    rz = p.rz();
    wrap_angles();
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
    Pose result(x + other.x, y + other.y, z + other.z, rx + other.rx, ry + other.ry, rz + other.rz);
    result.wrap_angles();
    return result;
}

Pose& Pose::operator+=(const Pose& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    rx += other.rx;
    ry += other.ry;
    rz += other.rz;
    wrap_angles();
    return *this;
}

Pose Pose::operator*(double f) const
{
    Pose result(x * f, y * f, z * f, rx * f, ry * f, rz * f);
    result.wrap_angles();
    return result;
}

Pose& Pose::operator*=(double f)
{
    x *= f;
    y *= f;
    z *= f;
    rx *= f;
    ry *= f;
    rz *= f;
    wrap_angles();
    return *this;
}

void Pose::wrap_angles()
{
    // do nothing for Pose
    // but Position is supposed to override
}

