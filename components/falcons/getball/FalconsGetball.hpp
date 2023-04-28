// this file was produced by MRA-codegen.py from template_instance.hpp
// it should NOT be modified by user

#ifndef _MRA_FALCONS_GETBALL_HPP
#define _MRA_FALCONS_GETBALL_HPP

#include "abstract_interface.hpp"

#include "components/falcons/getball/interface/Input.pb.h"
#include "components/falcons/getball/interface/Params.pb.h"
#include "components/falcons/getball/interface/Output.pb.h"


namespace MRA::FalconsGetball
{

typedef MRA::FalconsGetball::Input InputType;
typedef MRA::FalconsGetball::Params ParamsType;
typedef int StateType; // no .proto -> unused
typedef MRA::FalconsGetball::Output OutputType;
typedef int LocalType; // no .proto -> unused


class FalconsGetball: public MRAInterface<InputType, ParamsType, StateType, OutputType, LocalType>
{
public:
    FalconsGetball() {};
    ~FalconsGetball() {};

    // user implementation
    int tick(
        double            timestamp,   // simulation timestamp, seconds since start of simulation
        InputType  const &input,       // input data, type generated from Input.proto
        ParamsType const &params,      // configuration parameters, type generated from Params.proto
        StateType        &state,       // state data, type generated from State.proto
        OutputType       &output,      // output data, type generated from Output.proto
        LocalType        &local        // local/diagnostics data, type generated from Local.proto
    );

    // allow omitting parameters, useful for testing and code brevity
    int tick()
    {
        StateType s;
        OutputType o;
        LocalType l;
        return tick(0.0, InputType(), ParamsType(), s, o, l);
    };

    int tick(
        InputType  const &input,
        OutputType       &output
    )
    {
        StateType s;
        LocalType l;
        return tick(0.0, input, ParamsType(), s, output, l);
    };

    int tick(
        InputType  const &input,
        ParamsType const &params,
        OutputType       &output
    )
    {
        StateType s;
        LocalType l;
        return tick(0.0, input, params, s, output, l);
    };

}; // class FalconsGetball

} // namespace MRA::FalconsGetball

#endif

