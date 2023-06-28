// Copyright 2014-2022 Andre Pool and Geraldo Santiago
// SPDX-License-Identifier: Apache-2.0

#ifndef LINEPOINTDETECTION_HPP
#define LINEPOINTDETECTION_HPP

#include <vector>

namespace MRA::internal::FLocVis
{

struct angleRadiusSt {
    double angle;
    double radius;
    size_t xCamera;
    size_t yCamera;
    bool valid;
    bool operator<(const angleRadiusSt &val) const {
        // sorting this struct is performed on radius (distance)
        return radius > val.radius;
    }
};

class linePointDetection {

public:
    linePointDetection();
    std::vector<angleRadiusSt> getLinePointsPolar();

};

} // namespace MRA::internal::FLocVis

#endif
