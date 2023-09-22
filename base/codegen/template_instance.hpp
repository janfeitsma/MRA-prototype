// CODEGEN_NOTE
// it should NOT be modified by user

#ifndef _MRA_COMPONENT_CPP_NAME_UNDERSCORE_HPP
#define _MRA_COMPONENT_CPP_NAME_UNDERSCORE_HPP

#include "abstract_interface.hpp"
#include "params_loader.hpp"
#include <google/protobuf/empty.pb.h>
#include "logging.hpp"

// generated protobuf types from interface of this component
#include "datatypes.hpp"


// used in tracing
#ifdef MRA_COMPONENT_NAME
#undef MRA_COMPONENT_NAME
#endif
#define MRA_COMPONENT_NAME "COMPONENT_CPP_NAME_CAMELCASE"


namespace MRA::COMPONENT_CPP_NAME_CAMELCASE
{

PROTOBUF_HPP_TYPE_TYPEDEFS

class COMPONENT_CPP_NAME_CAMELCASE: public MRAInterface<InputType, ParamsType, StateType, OutputType, LocalType>
{
public:
    COMPONENT_CPP_NAME_CAMELCASE() {};
    ~COMPONENT_CPP_NAME_CAMELCASE() {};

    // user implementation
    int tick(
        google::protobuf::Timestamp timestamp,   // absolute timestamp
        InputType  const           &input,       // input data, type generated from Input.proto
        ParamsType const           &params,      // configuration parameters, type generated from Params.proto
        StateType                  &state,       // state data, type generated from State.proto
        OutputType                 &output,      // output data, type generated from Output.proto
        LocalType                  &local        // local/diagnostics data, type generated from Local.proto
    );

    // make default configuration easily accessible
    ParamsType defaultParams() const
    {
        return MRA::LoadDefaultParams<ParamsType>("components/COMPONENT_REL_PATH/interface/DefaultParams.json");
    };

    // allow omitting arguments, useful for testing and code brevity
    int tick()
    {
        StateType s;
        OutputType o;
        LocalType l;
        return tick(google::protobuf::util::TimeUtil::GetCurrentTime(), InputType(), defaultParams(), s, o, l);
    };

    int tick(
        InputType  const &input,
        OutputType       &output
    )
    {
        StateType s;
        LocalType l;
        return tick(google::protobuf::util::TimeUtil::GetCurrentTime(), input, defaultParams(), s, output, l);
    };

    int tick(
        InputType  const &input,
        ParamsType const &params,
        OutputType       &output
    )
    {
        StateType s;
        LocalType l;
        return tick(google::protobuf::util::TimeUtil::GetCurrentTime(), input, params, s, output, l);
    };

    int tick(
        InputType  const &input,
        ParamsType const &params,
        StateType        &state,
        OutputType       &output,
        LocalType        &local
    )
    {
        return tick(google::protobuf::util::TimeUtil::GetCurrentTime(), input, params, state, output, local);
    };

}; // class COMPONENT_CPP_NAME_CAMELCASE


// configuration handling
inline ParamsType defaultParams()
{
    return COMPONENT_CPP_NAME_CAMELCASE().defaultParams();
}
inline ParamsType loadParams(std::string configFile)
{
    return MRA::LoadDefaultParams<ParamsType>(configFile);
}

} // namespace MRA::COMPONENT_CPP_NAME_CAMELCASE


#endif

