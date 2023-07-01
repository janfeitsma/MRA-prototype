#include "solver.hpp"

// MRA libraries
#include "opencv_utils.hpp"

using namespace MRA::FalconsLocalizationVision;


Solver::Solver()
{
}

Solver::~Solver()
{
}

void Solver::configure(Params const &p)
{
    _params = p;
    // check for missing required parameters
    checkParamsValid();
    // configure helper classes
    _floor.configure(_params);
    _fit.configure(_params.solver());
}

void Solver::checkParamsValid()
{
    // check field model params
    if (_params.model().a() < 1)
    {
        throw std::runtime_error("invalid configuration: field model parameter A (field length in y) too small (got " + std::to_string(_params.model().a()) + ")");
    }
    if (_params.model().b() < 1)
    {
        throw std::runtime_error("invalid configuration: field model parameter B (field width in x) too small (got " + std::to_string(_params.model().b()) + ")");
    }
    if (_params.model().k() < 0.01)
    {
        throw std::runtime_error("invalid configuration: field model parameter K (line width) too small (got " + std::to_string(_params.model().k()) + ")");
    }
    // check solver params
    auto solverParams = _params.solver();
    if (solverParams.maxcount() < 1)
    {
        throw std::runtime_error("invalid configuration: maxCount should be a positive number (got " + std::to_string(solverParams.maxcount()) + ")");
    }
    if (solverParams.step().x() < 1)
    {
        throw std::runtime_error("invalid configuration: step.x too small (got " + std::to_string(solverParams.step().x()) + ")");
    }
    if (solverParams.step().y() < 1)
    {
        throw std::runtime_error("invalid configuration: step.y too small (got " + std::to_string(solverParams.step().y()) + ")");
    }
    if (solverParams.step().rz() < 1)
    {
        throw std::runtime_error("invalid configuration: step.rz too small (got " + std::to_string(solverParams.step().rz()) + ")");
    }
    if (solverParams.linepointradiusconstant() < 0.01)
    {
        throw std::runtime_error("invalid configuration: linePointRadiusConstant too small (got " + std::to_string(solverParams.linepointradiusconstant()) + ")");
    }
}

void Solver::set_state(State const &s)
{
    _state = s;
}

void Solver::set_input(Input const &in)
{
    _input = in;
}

void Solver::determine_reference_floor()
{
    // check if (re)calculation is needed
    if (_state.has_referencefloor()) return;

    // given the configured field (letter model and optional custom shapes),
    // create a CV::mat representation, using a blur factor
    // serialize and store the mat in state, as this should not be recalculated each tick
    // some external python plot tool should be able to plot it

    // determine set of shapes
    std::vector<MRA::Datatypes::Shape> shapes(_params.shapes().begin(), _params.shapes().end());
    if (_params.has_model())
    {
        _floor.letterModelToShapes(_params.model(), shapes); // TODO: move this to MRA::libraries? could be useful elsewhere
    }

    // create cv::Mat such that field is rotated screen-friendly: more columns than rows
    float blurFactor = _params.solver().blurfactor();
    cv::Mat m = _floor.createMat();
    _floor.shapesToCvMat(shapes, blurFactor, m);

    // store result as protobuf CvMatProto object
    MRA::OpenCVUtils::serializeCvMat(m, *_state.mutable_referencefloor());
}

int Solver::run()
{
    // TODO: multithreading?
    // TODO: on input, specify guesses and randomness etc
    // try to keep the design as simple as possible: minimize state, trackers (that is for worldModel to handle)
    // initially (and maybe also occasionally?) we should perhaps do some kind of grid search

    // get the reference floor
    cv::Mat referenceFloor;
    MRA::OpenCVUtils::deserializeCvMat(_state.referencefloor(), referenceFloor); // if not fast enough, then cache internally

    // create a floor (linePoints RCS, robot at (0,0,0)) for input linepoints
    cv::Mat rcsLinePoints = _floor.createMat();
    std::vector<Landmark> linePoints(_input.landmarks().begin(), _input.landmarks().end());
    _floor.linePointsToCvMat(linePoints, rcsLinePoints);

    // the core is a single fit operation (which uses opencv Downhill Simplex solver):
    // fit given white pixels and initial guess to the reference field
    FitResult r = _fit.run(referenceFloor, rcsLinePoints, _input.guess());

    // optional dump of diagnostics data for plotting
    if (_params.debug())
    {
        cv::Mat transformedLinePoints = rcsLinePoints; // TODO transform
        cv::Mat diagFloor = referenceFloor;
        MRA::OpenCVUtils::joinWhitePixels(diagFloor, transformedLinePoints);
        // add grid lines (all 1 pixel, so we can clearly see how the field line is positioned)
        _floor.addGridLines(diagFloor, 1.0, cv::Scalar(100, 100, 100)); // 1meter grid: very faint
        _floor.addGridLines(diagFloor, 2.0, cv::Scalar(200, 200, 200)); // 2meter grid: more prominent
        MRA::OpenCVUtils::serializeCvMat(diagFloor, *_diag.mutable_floor());
    }

    // process fit result
    if (r.success)
    {
        Candidate c;
        c.mutable_pose()->CopyFrom(r.pose);
        c.set_confidence(r.score);
        *_output.add_candidates() = c;
        return 0;
    }
    return 1;
}

Output const &Solver::get_output() const
{
    return _output;
}

Local const &Solver::get_diagnostics() const
{
    return _diag;
}

State const &Solver::get_state() const
{
    return _state;
}


