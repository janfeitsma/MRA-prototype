#ifndef _MRA_LIBRARIES_GEOMETRY_ANGLES_HPP
#define _MRA_LIBRARIES_GEOMETRY_ANGLES_HPP


namespace MRA::Geometry
{

// wrap rotation/angle to [0, 2pi) for standardized coordinates
double wrap_2pi(double rot);

// wrap rotation/angle to [-pi, pi) for subsequent abs()-and-compare
double wrap_pi(double rot);

// calculate relative angle in MSL-FCS, where 0 is facing opponent goal
// (which is 90 degrees rotated w.r.t. regular polar coordinate system)
// example: when robot at x=2 takes a kickoff, the angle towards ball is 0.5*pi
double calc_facing_angle_fcs(double from_x, double from_y, double to_x, double to_y);

} // namespace MRA::Geometry


#endif // #ifndef _MRA_LIBRARIES_GEOMETRY_ANGLES_HPP

