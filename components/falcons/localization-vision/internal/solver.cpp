#include "solver.hpp"


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

    // determine floor size
    float ppm = _params.solver().pixelspermeter();
    float sizeY = _params.model().a() + 2.0 * _params.solver().floorborder();
    int numPixelsY = int(sizeY * ppm);
    float sizeX = _params.model().b() + 2.0 * _params.solver().floorborder();
    int numPixelsX = int(sizeX * ppm);

    // configure helper class, sizes in FCS
    _floor.configure(sizeX, sizeY, ppm);

    // determine set of shapes
    std::vector<MRA::Datatypes::Shape> shapes(_params.shapes().begin(), _params.shapes().end());
    if (_params.has_model())
    {
        _floor.letterModelToShapes(_params.model(), shapes); // TODO: move this to MRA::libraries? could be useful elsewhere
    }

    // create cv::Mat such that field is rotated screen-friendly: more columns than rows
    float blurFactor = _params.solver().blurfactor();
    cv::Mat m = cv::Mat::zeros(numPixelsX, numPixelsY, CV_8UC1);
    _floor.shapesToCvMat(shapes, blurFactor, m);

    // store result as protobuf CvMatProto object
    _floor.serializeCvMat(m, *_state.mutable_referencefloor());
}

int Solver::run()
{
    // TODO: multithreading?
    // TODO: on input, specify guesses and randomness etc
    // try to keep the design as simple as possible: minimize state, trackers (that is for worldModel to handle)
    // initially (and maybe also occasionally?) we should perhaps do some kind of grid search

    // get the reference floor
    cv::Mat referenceFloor;
    _floor.deserializeCvMat(_state.referencefloor(), referenceFloor);

    // the core is a single fit operation (which uses opencv Downhill Simplex solver):
    // fit given white pixels and initial guess to the reference field
    _fit.settings.guess.x = -1.0;
    _fit.settings.guess.y = 3.0;
    _fit.settings.guess.rz = 2.0;
    FitResult r = _fit.run(referenceFloor);
    if (r.success)
    {
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


