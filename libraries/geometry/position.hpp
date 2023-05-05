#ifndef _MRA_LIBRARIES_GEOMETRY_POSITION_HPP
#define _MRA_LIBRARIES_GEOMETRY_POSITION_HPP

#include "pose.hpp"

namespace MRA::Geometry
{

// MRA::Geometry::Position is special version of Pose that adds coordinate transformations, 
//    ensuring angles are wrapped to [-pi,pi)

class Position: public Pose
{
public:
    Position(double x_=0.0, double y_=0.0, double z_=0.0, double rx_=0.0, double ry_=0.0, double rz_=0.0);
    Position(const MRA::Geometry::Pose& p);
    Position(const MRA::Datatypes::Pose& p);

    void wrap_angles() override;

    void addRcsToFcs(Position const &p); // add RCS offset to a FCS pose
    void faceAwayFrom(Position const &p);
    void faceTowards(Position const &p);

}; // class Position: public Position

} // namespace MRA::geometry

#endif // #ifndef _MRA_LIBRARIES_GEOMETRY_POSITION_HPP

