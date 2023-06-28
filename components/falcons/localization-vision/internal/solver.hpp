#ifndef _MRA_FALCONS_LOCALIZATION_VISION_SOLVER_HPP
#define _MRA_FALCONS_LOCALIZATION_VISION_SOLVER_HPP

#include "datatypes.hpp"

namespace MRA::FalconsLocalizationVision
{

class Solver
{
public:
    Solver();
    ~Solver();
    void configure(Params const &p);
    void set_state(State const &s);
    void set_input(Input const &in);
    void determine_reference_floor();

    int run();

    Output const &get_output() const;
    Local const &get_diagnostics() const;

private:
    Input  _input;
    Params _params;
    State  _state;
    Output _output;
    Local  _diag;

}; // class Solver

} // namespace MRA::FalconsLocalizationVision

#endif

