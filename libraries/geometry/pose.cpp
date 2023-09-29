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

Pose::Pose(MRA::Datatypes::Pose const &p)
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
void Pose::reset()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
    rx = 0.0;
    ry = 0.0;
    rz = 0.0;
}

double Pose::size() const
{
    return sqrt(x * x + y * y + z * z);
}

std::string Pose::xyrz_str() const
{
    char buffer[256];
    sprintf(buffer, " %9.4f %9.4f %9.4f", x, y, rz);
    return buffer;
}

bool Pose::is_zero(double tolerance) const
{
    if (abs(x) > tolerance) return false;
    if (abs(y) > tolerance) return false;
    if (abs(z) > tolerance) return false;
    if (abs(rx) > tolerance) return false;
    if (abs(ry) > tolerance) return false;
    if (abs(rz) > tolerance) return false;
    return true;
}

Pose Pose::operator+(Pose const &other) const
{
    Pose result(x + other.x, y + other.y, z + other.z, rx + other.rx, ry + other.ry, rz + other.rz);
    result.wrap_angles();
    return result;
}

Pose& Pose::operator+=(Pose const &other)
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

Pose Pose::operator-(Pose const &other) const
{
    Pose result(x - other.x, y - other.y, z - other.z, rx - other.rx, ry - other.ry, rz - other.rz);
    result.wrap_angles();
    return result;
}

Pose& Pose::operator-=(Pose const &other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    rx -= other.rx;
    ry -= other.ry;
    rz -= other.rz;
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

Pose Pose::operator/(double f) const
{
    return operator*(1.0 / f);
}

Pose& Pose::operator/=(double f)
{
    return operator*=(1.0 / f);
}

Pose::operator MRA::Geometry::Point() const
{
    return MRA::Geometry::Point(x, y);
}

Pose::operator MRA::Datatypes::Point() const
{
    MRA::Datatypes::Point result;
    result.set_x(x);
    result.set_y(y);
    return result;
}

Pose::operator MRA::Datatypes::Pose() const
{
    MRA::Datatypes::Pose result;
    result.set_x(x);
    result.set_y(y);
    result.set_rz(rz);
    return result;
}

void Pose::wrap_angles()
{
    // do nothing for Pose
    // but Position is supposed to override
}

