#include "position.hpp"
#include "clipping.hpp"
#include <cmath>

using namespace MRA::Geometry;


// constructors, destructor
Position::Position(double x_, double y_, double z_, double rx_, double ry_, double rz_)
{
    x = x_;
    y = y_;
    z = z_;
    rx = rx_;
    ry = ry_;
    rz = rz_;
    wrap_angles();
}

Position::Position(MRA::Datatypes::Pose const &p)
{
    x = p.x();
    y = p.y();
    z = p.z();
    rx = p.rx();
    ry = p.ry();
    rz = p.rz();
    wrap_angles();
}

Position::Position(MRA::Geometry::Pose const &p)
{
    x = p.x;
    y = p.y;
    z = p.z;
    rx = p.rx;
    ry = p.ry;
    rz = p.rz;
    wrap_angles();
}

// Position requires angle wrapping
void Position::wrap_angles()
{
    rx = wrap_pi(rx);
    ry = wrap_pi(ry);
    rz = wrap_pi(rz);
}

// add RCS offset to a FCS pose
Position& Position::addRcsToFcs(Position const &p)
{
    Position result = p;
    result.transformRcsToFcs(*this);
    *this = result;
    return *this;
}

#include "tmp_vector2d.hpp" // TODO: get rid of this (comes from Falcons legacy), make it nicer or consider using Eigen

// transform (planar) between RCS and FCS using reference pos in FCS
Position& Position::transformRcsToFcs(Position const &refpos)
{
    // first rotate, then translate
    double angle = refpos.rz;
    Vector2D xyrot = (Vector2D(x, y)).rotate(angle);
    x = xyrot.x + refpos.x;
    y = xyrot.y + refpos.y;
    rz += angle;
    wrap_angles();
    return *this;
}

Position& Position::transformFcsToRcs(Position const &refpos)
{
    // first translate, then rotate
    double angle = -refpos.rz;
    Vector2D xynew = (Vector2D(x, y) - Vector2D(refpos.x, refpos.y)).rotate(angle);
    x = xynew.x;
    y = xynew.y;
    rz += angle;
    wrap_angles();
    return *this;
}

Position& Position::faceAwayFrom(Position const &p)
{
    rz = atan2(y - p.y, x - p.x);
    wrap_angles(); // technically not needed as long as we stick to [-pi,pi)
    return *this;
}

Position& Position::faceTowards(Position const &p)
{
    rz = atan2(p.y - y, p.x - x);
    wrap_angles(); // technically not needed as long as we stick to [-pi,pi)
    return *this;
}

