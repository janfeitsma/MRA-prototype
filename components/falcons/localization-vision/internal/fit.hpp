#ifndef _MRA_FALCONS_LOCALIZATION_VISION_FIT_HPP
#define _MRA_FALCONS_LOCALIZATION_VISION_FIT_HPP

#include <opencv2/core/optim.hpp>
#include "geometry.hpp"

// generated protobuf types from interface of this component
#include "datatypes.hpp"


namespace MRA::FalconsLocalizationVision
{

struct FitResult
{
    bool success = false;
    float score = 0.0;
    MRA::Geometry::Pose pose;
    bool operator<(FitResult const &other) { return score < other.score; }
}; // struct FitResult


class FitFunction: public cv::MinProblemSolver::Function
{
public:
    FitFunction(cv::Mat const &referenceFloor, cv::Mat const &rcsLinePoints, float ppm);
	double calc(const double *x) const; // this is the main scoring function to be minimized, x is a tuple (x,y,rz)
    int getDims() const { return 3; }

    // helpers, public for testing purposes and diagnostics
    double calcOverlap(cv::Mat const &m1, cv::Mat const &m2) const;
    cv::Mat transform3dof(cv::Mat const &m, double x, double y, double rz) const;

private:
    cv::Mat _referenceFloor;
    cv::Mat _rcsLinePoints;
    double _rcsLinePointsPixelCount = 1.0; // for score normalization
    float _ppm; // needed to optimize in FCS instead of pixels
}; // class FitFunction


class FitCore
{
public:
    FitCore() {};
    ~FitCore() {};

    SolverParams settings;
    void configure(SolverParams const &config) { settings.CopyFrom(config); }

    FitResult run(
        cv::Mat const &referenceFloor,      // params translated once (at first tick) to reference floor to fit against, white pixels, potentially blurred
        cv::Mat const &rcsLinePoints,       // input pixels translated to a floor that can be compared / fitted
        MRA::Datatypes::Pose const &guess,  // initial guess for the algorithm, note that the simplex is constructed AROUND it, so somewhere a shift might be needed
        MRA::Datatypes::Pose const &step);  // initial step: search region

}; // class FitCore


class FitAlgorithm
{
public:
    FitAlgorithm() {};
    ~FitAlgorithm() {};

    SolverParams settings;
    void configure(SolverParams const &config) { settings.CopyFrom(config); _fitCore.configure(config); }

    FitResult run(
        cv::Mat const &referenceFloor,      // params translated once (at first tick) to reference floor to fit against, white pixels, potentially blurred
        cv::Mat const &rcsLinePoints,       // input pixels translated to a floor that can be compared / fitted
        MRA::Datatypes::Pose const &inputGuess,  // initial guess for the algorithm, note that the simplex is constructed AROUND it, so somewhere a shift might be needed
        std::vector<MRA::Datatypes::Circle> const &extraGuesses); // extra guesses (with range) if so configured

private:
    FitCore _fitCore;

}; // class FitAlgorithm


} // namespace MRA::FalconsLocalizationVision

#endif

