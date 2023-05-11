#ifndef _MRA_LIBRARIES_GEOMETRY_CLIPPING_HPP
#define _MRA_LIBRARIES_GEOMETRY_CLIPPING_HPP


namespace MRA::Geometry
{

// wrap rotation/angle to [0, 2pi) for standardized coordinates
double wrap_2pi(double rot);

// wrap rotation/angle to [-pi, pi) for subsequent abs()-and-compare
double wrap_pi(double rot);

} // namespace MRA::Geometry


#endif // #ifndef _MRA_LIBRARIES_GEOMETRY_CLIPPING_HPP

