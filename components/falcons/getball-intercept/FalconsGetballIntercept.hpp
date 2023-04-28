// this file was produced by MRA-codegen.py from template_instance.hpp
// it should NOT be modified by user

#ifndef _MRA_FALCONS_GETBALL_INTERCEPT_HPP
#define _MRA_FALCONS_GETBALL_INTERCEPT_HPP

#include "abstract_interface.hpp"

#include "components/falcons/getball-intercept/interface/Input.pb.h"
#include "components/falcons/getball-intercept/interface/Params.pb.h"
#include "components/falcons/getball-intercept/interface/Output.pb.h"


namespace MRA::FalconsGetballIntercept
{

typedef MRA::FalconsGetballIntercept::Input InputType;
typedef MRA::FalconsGetballIntercept::Params ParamsType;
typedef int StateType; // no .proto -> unused
typedef MRA::FalconsGetballIntercept::Output OutputType;
typedef int LocalType; // no .proto -> unused


class FalconsGetballIntercept: public MRAInterface<InputType, ParamsType, StateType, OutputType, LocalType>
{
public:
    FalconsGetballIntercept() {};
    ~FalconsGetballIntercept() {};

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

}; // class FalconsGetballIntercept

} // namespace MRA::FalconsGetballIntercept

#endif

