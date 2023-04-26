// this file was produced by codegen.py from template_instance.hpp
// it should NOT be modified by user

#ifndef _BEAST_MRA_FALCONS_GETBALL_FETCH_HPP
#define _BEAST_MRA_FALCONS_GETBALL_FETCH_HPP

#include "abstract_interface.hpp"

#include "MRA/components/falcons/getball-fetch/interface/Input.pb.h"
#include "MRA/components/falcons/getball-fetch/interface/Params.pb.h"
#include "MRA/components/falcons/getball-fetch/interface/Output.pb.h"


namespace FalconsGetballFetch
{

typedef FalconsGetballFetch::Input InputType;
typedef FalconsGetballFetch::Params ParamsType;
typedef int StateType; // no .proto -> unused
typedef FalconsGetballFetch::Output OutputType;
typedef int LocalType; // no .proto -> unused


class FalconsGetballFetch: public MRAInterface<InputType, ParamsType, StateType, OutputType, LocalType>
{
public:
    FalconsGetballFetch() {};
    ~FalconsGetballFetch() {};

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

}; // class FalconsGetballFetch

} // namespace FalconsGetballFetch

#endif

