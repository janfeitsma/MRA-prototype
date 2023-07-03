#ifndef _MRA_FALCONS_LOCALIZATION_VISION_SOLVER_HPP
#define _MRA_FALCONS_LOCALIZATION_VISION_SOLVER_HPP

#include "datatypes.hpp"
#include "floor.hpp"
#include "fit.hpp"

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
    cv::Mat createReferenceFloorMat(float blurFactor = 0.0) const;

    // input linepoints mat: calculate each tick, based on input landmarks / linepoints
    cv::Mat _linePointsMat;
    cv::Mat createLinePointsMat(float overruleRadius = 0.0) const;

    // control the guessing procedure
    std::vector<MRA::Datatypes::Circle> createExtraGuesses() const;

    // optional debug data export
    cv::Mat createDiagnosticsMat() const;

}; // class Solver

} // namespace MRA::FalconsLocalizationVision

#endif

