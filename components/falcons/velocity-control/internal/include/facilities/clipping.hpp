/*
 * clipping.hpp
 *
 *  Created on: Nov, 2019
 *      Author: Jan Feitsma
 */

#ifndef CLIPPING_HPP_
#define CLIPPING_HPP_

// MRA-libraries
#include "MRAbridge.hpp"


// adapter for template clip()
inline double fabs(Vector2D const &v)
{
    return v.size();
}

// generalized clipping of a value v to an area around p with radius r
// v may be a float or vector2d
template <typename T>
bool gclip(T &v, T const p, float r, char const *label = "")
{
    r = fabs(r);
    T vo = v;
    T delta = v - p;
    if ((fabs(delta) > r) && (fabs(delta) > 0.1))
    {
        // normalize
        delta *= r / fabs(delta);
        v = p + delta;
        return true;
    }
    return false;
}

#endif

