// Copyright 2014-2022 Andre Pool
// SPDX-License-Identifier: Apache-2.0

#ifndef FIELDLUT_HPP
#define FIELDLUT_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/core/optim.hpp>

#include "configurator.hpp"
#include "linePointDetection.hpp"
#include "robotFloor.hpp"

class fieldLut: public cv::MinProblemSolver::Function {

private:
	// pointers for access to other classes
	configurator *conf;
	linePointDetection *linePoint;
	robotFloor *rFloor;

	int pixelThreshold;

	void initializeFrameCost();

public:
	fieldLut(configurator *conf, linePointDetection *linePoint, robotFloor *rFloor);
	double calc(const double *x) const;
    int getDims() const { return 3; }
	scoreStruct calcSimple(positionStDbl pos, double threshold);
};

#endif
