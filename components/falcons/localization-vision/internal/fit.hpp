#ifndef _MRA_FALCONS_LOCALIZATION_VISION_FIT_HPP
#define _MRA_FALCONS_LOCALIZATION_VISION_FIT_HPP

#include <opencv2/core/optim.hpp>

// generated protobuf types from interface of this component
#include "datatypes.hpp"


namespace MRA::FalconsLocalizationVision
{

struct FitResult
{
    bool success = false;
    float score = 0.0;
    MRA::Datatypes::Pose pose;
    cv::Mat floor;
}; // struct FitResult


class FitFunction: public cv::MinProblemSolver::Function
{
public:
    FitFunction(cv::Mat const &referenceFloor, cv::Mat const &rcsLinePoints);
	double calc(const double *x) const;
    int getDims() const { return 3; }
private:
    cv::Mat _referenceFloor;
    cv::Mat _rcsLinePoints;
}; // class FitFunction


class FitAlgorithm
{
public:
    FitAlgorithm();
    ~FitAlgorithm();

    SolverParams settings;
    void configure(SolverParams const &config) { settings.CopyFrom(config); }

    FitResult run(cv::Mat const &referenceFloor, cv::Mat const &rcsLinePoints, MRA::Datatypes::Pose const &guess);

}; // class FitAlgorithm


} // namespace MRA::FalconsLocalizationVision

#endif

