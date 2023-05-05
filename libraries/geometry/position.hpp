#ifndef _MRA_LIBRARIES_GEOMETRY_POSITION_HPP
#define _MRA_LIBRARIES_GEOMETRY_POSITION_HPP

#include "pose.hpp"

namespace MRA::Geometry
{

// MRA::Geometry::Position is special version of Pose that adds coordinate transformations, 
//    ensuring angles are within valid range [0,2pi) using clip_2pi

class Position: public Pose
{
public:
    Position() : Pose() {};
    Position(const MRA::Geometry::Pose& p) : Pose(p) {};
    Position(const MRA::Datatypes::Pose& p) : Pose(p) {};

    void clip(); // clip angles

    void addRcsToFcs(Position const &p); // add RCS offset to a FCS pose
    void faceAwayFrom(Position const &p);
    void faceTowards(Position const &p);

}; // class Position: public Position

} // namespace MRA::geometry

#endif // #ifndef _MRA_LIBRARIES_GEOMETRY_POSITION_HPP

