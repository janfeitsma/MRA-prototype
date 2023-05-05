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

Position::Position(const MRA::Datatypes::Pose& p)
{
    x = p.x();
    y = p.y();
    z = p.z();
    rx = p.rx();
    ry = p.ry();
    rz = p.rz();
    wrap_angles();
}

Position::Position(const MRA::Geometry::Pose& p)
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

void Position::faceAwayFrom(Position const &p)
{
    rz = atan2(y - p.y, x - p.x);
    wrap_angles();
}

void Position::faceTowards(Position const &p)
{
    rz = atan2(p.y - y, p.x - x);
    wrap_angles();
}

// add RCS offset to a FCS pose
void Position::addRcsToFcs(Position const &p)
{
}
/*
Position2D addRcsToFcs(Position2D const &posRcs, Position2D const &posFcs)
{
    // return posFcs with posRcs offset added
    // TODO (#14): awkward old Position2D API ... we should actually improve the core Position2D class ...
    Position2D result = posRcs;
    result.transform_rcs2fcs(posFcs);
    result.phi = posFcs.phi;
    return result;
}
*/
/*
Position2D& Position2D::transform_fcs2rcs(const Position2D& robotpos)
{  
    // first ttranslate, then rotate
    double angle = (M_PI_2 - robotpos.phi);
    Vector2D xynew = (Vector2D(x, y) - robotpos.xy()).rotate(angle);
    x = xynew.x;
    y = xynew.y;
    phi = phi + angle;
    phi = project_angle_0_2pi(phi);
    return (*this);
}

Position2D& Position2D::transform_rcs2fcs(const Position2D& robotpos)
{
    // first rotate, then translate
    double angle = - (M_PI_2 - robotpos.phi);
    Vector2D xyrot = (Vector2D(x, y)).rotate(angle);
    x = xyrot.x + robotpos.x;
    y = xyrot.y + robotpos.y;
    phi = phi + angle;
    phi = project_angle_0_2pi(phi);
    return (*this);
}
*/

