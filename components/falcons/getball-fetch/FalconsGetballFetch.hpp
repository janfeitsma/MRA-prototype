// this file was produced by MRA-codegen.py from template_instance.hpp
// it should NOT be modified by user

#ifndef _MRA_FALCONS_GETBALL_FETCH_HPP
#define _MRA_FALCONS_GETBALL_FETCH_HPP

#include "abstract_interface.hpp"
#include "params_loader.hpp"

#include "components/falcons/getball-fetch/interface/Input.pb.h"
#include "components/falcons/getball-fetch/interface/Params.pb.h"
#include "components/falcons/getball-fetch/interface/Output.pb.h"


namespace MRA::FalconsGetballFetch
{

typedef MRA::FalconsGetballFetch::Input InputType;
typedef MRA::FalconsGetballFetch::Params ParamsType;
typedef int StateType; // no .proto -> unused
typedef MRA::FalconsGetballFetch::Output OutputType;
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
        return tick(0.0, InputType(), MRA::LoadDefaultParams<ParamsType>("components/falcons/getball-fetch/interface/DefaultParams.json"), s, o, l);
    };

    int tick(
        InputType  const &input,
        OutputType       &output
    )
    {
        StateType s;
        LocalType l;
        return tick(0.0, input, MRA::LoadDefaultParams<ParamsType>("components/falcons/getball-fetch/interface/DefaultParams.json"), s, output, l);
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

} // namespace MRA::FalconsGetballFetch

#endif

