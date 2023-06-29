#ifndef _MRA_FALCONS_LOCALIZATION_VISION_FIT_HPP
#define _MRA_FALCONS_LOCALIZATION_VISION_FIT_HPP

#include <opencv2/core/optim.hpp>


namespace MRA::FalconsLocalizationVision
{

struct FitPose
{
    float x;
    float y;
    float rz;
};

struct FitSettings
{
    FitPose guess;
}; // struct FitSettings


struct FitResult
{
    bool success = false;
    float score = 0.0;
    FitPose pose;
}; // struct FitResult


class FitFunction: public cv::MinProblemSolver::Function
{
public:
    FitFunction();
	double calc(const double *x) const;
    int getDims() const { return 3; }
}; // class FitFunction


class FitAlgorithm
{
public:
    FitAlgorithm();
    ~FitAlgorithm();

    FitSettings settings;

    FitResult run(cv::Mat const &referenceFloor);

}; // class FitAlgorithm


} // namespace MRA::FalconsLocalizationVision

#endif

