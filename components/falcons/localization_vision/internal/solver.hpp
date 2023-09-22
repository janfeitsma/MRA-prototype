#ifndef _MRA_FALCONS_LOCALIZATION_VISION_SOLVER_HPP
#define _MRA_FALCONS_LOCALIZATION_VISION_SOLVER_HPP

#include "datatypes.hpp"
#include "floor.hpp"
#include "fit.hpp"
#include "tracker.hpp"

namespace MRA::FalconsLocalizationVision
{

class Solver
{
public:
    Solver();
    ~Solver();

    void configure(Params const &p);
    void setState(State const &s);
    void setInput(Input const &in);

    int run();

    Output const &getOutput() const;
    Local const &getDiagnostics() const;
    State const &getState() const;

private:
    Input  _input;
    Params _params;
    State  _state;
    Output _output;
    Local  _diag;

    FitResult _fitResult;

    // checks at config time (typically once right after construction, or more often when tuning)
    void checkParamsValid() const;

    // helper classes
    Floor  _floor;
    FitAlgorithm _fitAlgorithm;
    void configureFloor();
    void configureFit();

    // reference floor: calculate once, based on letter model and optional extra shapes
    cv::Mat _referenceFloorMat;
public:
    cv::Mat createReferenceFloorMat(float blurFactor = 0.0) const;

private:
    // initialization (a bit expensive), only once, or when parameters change
    bool _reinit = false;
    void reinitialize();

    // input linepoints: calculate each tick, based on input landmarks / linepoints
    std::vector<cv::Point2f> _linePoints;
    std::vector<cv::Point2f> createLinePoints() const;

    // setup trackers: existing ones from state and new ones from guessing configuration
    std::vector<Tracker> _trackers;
    std::vector<Tracker> createTrackers() const;

    // run the fit algorithm (multithreaded) and update trackers
    void runFitUpdateTrackers();
    void cleanupBadTrackers();

    // optional debug data export
    cv::Mat createDiagnosticsMat() const;
    void dumpDiagnosticsMat();

}; // class Solver

} // namespace MRA::FalconsLocalizationVision

#endif

