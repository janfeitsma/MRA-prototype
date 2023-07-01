#include "point.hpp"
#include <cmath>

using namespace MRA::Geometry;


// constructors, destructor
Point::Point(double x_, double y_)
{
    x = x_;
    y = y_;
}

Point::Point(MRA::Datatypes::Point const &p)
{
    x = p.x();
    y = p.y();
}

Point::operator MRA::Datatypes::Point() const
{
    MRA::Datatypes::Point result;
    result.set_x(x);
    result.set_y(y);
    return result;
}

Point::~Point()
{
}

// basic operations
void Point::reset()
{
    x = 0.0;
    y = 0.0;
}

double Point::size() const
{
    return sqrt(x * x + y * y);
}    

Point Point::operator+(Point const &other) const
{
    Point result(x + other.x, y + other.y);
    return result;
}

Point& Point::operator+=(Point const &other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Point Point::operator-(Point const &other) const
{
    Point result(x - other.x, y - other.y);
    return result;
}

Point& Point::operator-=(Point const &other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

Point Point::operator*(double f) const
{
    Point result(x * f, y * f);
    return result;
}

Point& Point::operator*=(double f)
{
    x *= f;
    y *= f;
    return *this;
}

Point Point::operator/(double f) const
{
    return operator*(1.0 / f);
}

Point& Point::operator/=(double f)
{
    return operator*=(1.0 / f);
}

