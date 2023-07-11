// this file was produced by MRA-codegen.py from template_instance.hpp
// it should NOT be modified by user

#ifndef _MRA_ROBOTSPORTS_GETBALL_INTERCEPT_HPP
#define _MRA_ROBOTSPORTS_GETBALL_INTERCEPT_HPP

#include "abstract_interface.hpp"
#include "params_loader.hpp"
#include <google/protobuf/empty.pb.h>

// generated protobuf types from interface of this component
#include "datatypes.hpp"

namespace MRA::RobotsportsGetballIntercept
{

typedef MRA::RobotsportsGetballIntercept::Input InputType;
typedef MRA::RobotsportsGetballIntercept::Params ParamsType;
typedef google::protobuf::Empty StateType; // no .proto -> unused
typedef MRA::RobotsportsGetballIntercept::Output OutputType;
typedef google::protobuf::Empty LocalType; // no .proto -> unused


class RobotsportsGetballIntercept: public MRAInterface<InputType, ParamsType, StateType, OutputType, LocalType>
{
public:
    RobotsportsGetballIntercept() {};
    ~RobotsportsGetballIntercept() {};

    // user implementation
    int tick(
        double            timestamp,   // simulation timestamp, seconds since start of simulation
        InputType  const &input,       // input data, type generated from Input.proto
        ParamsType const &params,      // configuration parameters, type generated from Params.proto
        StateType        &state,       // state data, type generated from State.proto
        OutputType       &output,      // output data, type generated from Output.proto
        LocalType        &local        // local/diagnostics data, type generated from Local.proto
    );

    // make default configuration easily accessible
    ParamsType defaultParams() const
    {
        return MRA::LoadDefaultParams<ParamsType>("components/robotsports/getball-intercept/interface/DefaultParams.json");
    };

    // allow omitting arguments, useful for testing and code brevity
    int tick()
    {
        StateType s;
        OutputType o;
        LocalType l;
        return tick(0.0, InputType(), defaultParams(), s, o, l);
    };

    int tick(
        InputType  const &input,
        OutputType       &output
    )
    {
        StateType s;
        LocalType l;
        return tick(0.0, input, defaultParams(), s, output, l);
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

}; // class RobotsportsGetballIntercept


// configuration handling
inline ParamsType defaultParams()
{
    return RobotsportsGetballIntercept().defaultParams();
}
inline ParamsType loadParams(std::string configFile)
{
    return MRA::LoadDefaultParams<ParamsType>(configFile);
}

} // namespace MRA::RobotsportsGetballIntercept

#endif

