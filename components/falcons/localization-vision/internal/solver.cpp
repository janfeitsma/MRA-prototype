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

