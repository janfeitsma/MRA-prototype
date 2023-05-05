#include "position.hpp"
#include "clipping.hpp"
#include <cmath>

using namespace MRA::Geometry;


// Position requires clipping
void Position::clip()
{
    rx = clip_2pi(rx);
    ry = clip_2pi(ry);
    rz = clip_2pi(rz);
}

void Position::faceAwayFrom(Position const &p)
{
    rz = clip_2pi(atan2(y - p.y, x - p.x));
}

void Position::faceTowards(Position const &p)
{
    rz = clip_2pi(atan2(p.y - y, p.x - x));
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

