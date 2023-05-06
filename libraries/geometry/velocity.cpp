#include "velocity.hpp"

using namespace MRA::Geometry;


// constructors, destructor
Velocity::Velocity(double x_, double y_, double z_, double rx_, double ry_, double rz_)
{
    x = x_;
    y = y_;
    z = z_;
    rx = rx_;
    ry = ry_;
    rz = rz_;
}

Velocity::Velocity(MRA::Datatypes::Pose const &p)
{
    x = p.x();
    y = p.y();
    z = p.z();
    rx = p.rx();
    ry = p.ry();
    rz = p.rz();
}

Velocity::Velocity(MRA::Geometry::Pose const &p)
{
    x = p.x;
    y = p.y;
    z = p.z;
    rx = p.rx;
    ry = p.ry;
    rz = p.rz;
}

#include "tmp_vector2d.hpp" // TODO: get rid of this (comes from Falcons legacy), make it nicer or consider using Eigen

// transform (planar) between RCS and FCS using reference pos in FCS
Velocity& Velocity::transformRcsToFcs(Velocity const &refpos)
{
    double angle = refpos.rz;
    Vector2D xynew = Vector2D(x, y).rotate(angle);
    x = xynew.x;
    y = xynew.y;
    return *this;
}

Velocity& Velocity::transformFcsToRcs(Velocity const &refpos)
{
    double angle = -refpos.rz;
    Vector2D xynew = Vector2D(x, y).rotate(angle);
    x = xynew.x;
    y = xynew.y;
    return *this;
}

