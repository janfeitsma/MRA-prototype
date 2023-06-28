#include "solver.hpp"
#include "floor.hpp"

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

    // create helper class
    Floor floor;

    // determine set of shapes
    std::vector<MRA::Datatypes::Shape> shapes(_params.shapes().begin(), _params.shapes().end());
    if (_params.has_model())
    {
        floor.letterModelToShapes(_params.model(), shapes); // TODO: move this to MRA::libraries? could be useful elsewhere
    }

    // create cv::Mat
    float blurFactor = _params.solver().blurfactor();
    float sizeY = _params.model().a() + _params.solver().floorborder();
    int numPixelsY = int(sizeY * _params.solver().pixelspermeter());
    float sizeX = _params.model().b() + _params.solver().floorborder();
    int numPixelsX = int(sizeX * _params.solver().pixelspermeter());
    cv::Mat m = cv::Mat::zeros(numPixelsX, numPixelsY, CV_8UC1);
    floor.shapesToCvMat(shapes, blurFactor, m);

    // store result
    floor.serializeCvMat(m, _state.mutable_referencefloor());
}

int Solver::run()
{
    return 0;
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


