#ifndef _MRA_LIBRARIES_GEOMETRY_POSITION_HPP
#define _MRA_LIBRARIES_GEOMETRY_POSITION_HPP

#include "pose.hpp"

namespace MRA::Geometry
{

// MRA::Geometry::Position is special version of Pose that adds coordinate transformations,
//    ensuring angles are wrapped to [-pi,pi)
//    NOTE: this deviates from the 2014 standard of wrapping rz to [0,2pi), for the following reasons:
//    1. practical, ease of coding (for instance abs-and-compare)
//    2. better fits with generalized 6dof approach

class Position: public Pose
{
public:
    Position(double x_=0.0, double y_=0.0, double z_=0.0, double rx_=0.0, double ry_=0.0, double rz_=0.0);
    Position(MRA::Geometry::Pose const &p);
    Position(MRA::Datatypes::Pose const &p);

    // wrapping is implemented for Position (but not for Pose)
    void wrap_angles() override;

    // transform (planar) between RCS and FCS using refpos in FCS
    Position& transformRcsToFcs(Position const &refpos);
    Position& transformFcsToRcs(Position const &refpos);

    // add RCS offset to a FCS pose
    Position& addRcsToFcs(Position const &p);

    // set rz based on another position
    Position& faceAwayFrom(Position const &p);
    Position& faceTowards(Position const &p);

}; // class Position: public Position

} // namespace MRA::geometry

#endif // #ifndef _MRA_LIBRARIES_GEOMETRY_POSITION_HPP

