// this file was produced by MRA-codegen.py from template_instance.hpp
// it should NOT be modified by user

#ifndef _MRA_FALCONS_TRAJECTORY_GENERATION_HPP
#define _MRA_FALCONS_TRAJECTORY_GENERATION_HPP

#include "abstract_interface.hpp"
#include "params_loader.hpp"
#include <google/protobuf/empty.pb.h>

// generated protobuf types from interface of this component
#include "datatypes.hpp"

namespace MRA::FalconsTrajectoryGeneration
{

typedef MRA::FalconsTrajectoryGeneration::Input InputType;
typedef MRA::FalconsTrajectoryGeneration::Params ParamsType;
typedef google::protobuf::Empty StateType; // no .proto -> unused
typedef MRA::FalconsTrajectoryGeneration::Output OutputType;
typedef google::protobuf::Empty LocalType; // no .proto -> unused


class FalconsTrajectoryGeneration: public MRAInterface<InputType, ParamsType, StateType, OutputType, LocalType>
{
public:
    FalconsTrajectoryGeneration() {};
    ~FalconsTrajectoryGeneration() {};

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
        return MRA::LoadDefaultParams<ParamsType>("components/falcons/trajectory-generation/interface/DefaultParams.json");
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

}; // class FalconsTrajectoryGeneration

} // namespace MRA::FalconsTrajectoryGeneration

#endif

