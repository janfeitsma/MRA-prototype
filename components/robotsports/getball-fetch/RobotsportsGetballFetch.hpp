// this file was produced by MRA-codegen.py from template_instance.hpp
// it should NOT be modified by user

#ifndef _MRA_ROBOTSPORTS_GETBALL_FETCH_HPP
#define _MRA_ROBOTSPORTS_GETBALL_FETCH_HPP

#include "abstract_interface.hpp"
#include "params_loader.hpp"

#include "components/robotsports/getball-fetch/interface/Input.pb.h"
#include "components/robotsports/getball-fetch/interface/Params.pb.h"
#include "components/robotsports/getball-fetch/interface/Output.pb.h"


namespace MRA::RobotsportsGetballFetch
{

typedef MRA::RobotsportsGetballFetch::Input InputType;
typedef MRA::RobotsportsGetballFetch::Params ParamsType;
typedef int StateType; // no .proto -> unused
typedef MRA::RobotsportsGetballFetch::Output OutputType;
typedef int LocalType; // no .proto -> unused


class RobotsportsGetballFetch: public MRAInterface<InputType, ParamsType, StateType, OutputType, LocalType>
{
public:
    RobotsportsGetballFetch() {};
    ~RobotsportsGetballFetch() {};

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
        return tick(0.0, InputType(), MRA::LoadDefaultParams<ParamsType>("components/robotsports/getball-fetch/interface/DefaultParams.json"), s, o, l);
    };

    int tick(
        InputType  const &input,
        OutputType       &output
    )
    {
        StateType s;
        LocalType l;
        return tick(0.0, input, MRA::LoadDefaultParams<ParamsType>("components/robotsports/getball-fetch/interface/DefaultParams.json"), s, output, l);
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

}; // class RobotsportsGetballFetch

} // namespace MRA::RobotsportsGetballFetch

#endif

