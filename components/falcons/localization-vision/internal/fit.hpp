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
    cv::Mat floor;
}; // struct FitResult


class FitFunction: public cv::MinProblemSolver::Function
{
public:
    FitFunction(cv::Mat const &referenceFloor, cv::Mat const &rcsLinePoints);
	double calc(const double *x) const;
    int getDims() const { return 3; }
    cv::Mat getFloor() const { return _lastFloor; }
private:
    cv::Mat _referenceFloor;
    cv::Mat _rcsLinePoints;
    cv::Mat _lastFloor;
}; // class FitFunction


class FitAlgorithm
{
public:
    FitAlgorithm();
    ~FitAlgorithm();

    FitSettings settings;

    FitResult run(cv::Mat const &referenceFloor, cv::Mat const &rcsLinePoints);

}; // class FitAlgorithm


} // namespace MRA::FalconsLocalizationVision

#endif

