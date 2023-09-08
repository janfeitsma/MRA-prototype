// this file was produced by MRA-codegen.py from template_instance.hpp
// it should NOT be modified by user

#ifndef _MRA_FALCONS_LOCALIZATION_VISION_HPP
#define _MRA_FALCONS_LOCALIZATION_VISION_HPP

#include "abstract_interface.hpp"
#include "params_loader.hpp"
#include <google/protobuf/empty.pb.h>

// generated protobuf types from interface of this component
#include "datatypes.hpp"

namespace MRA::FalconsLocalizationVision
{

typedef MRA::FalconsLocalizationVision::Input InputType;
typedef MRA::FalconsLocalizationVision::Params ParamsType;
typedef MRA::FalconsLocalizationVision::State StateType;
typedef MRA::FalconsLocalizationVision::Output OutputType;
typedef MRA::FalconsLocalizationVision::Local LocalType;


class FalconsLocalizationVision: public MRAInterface<InputType, ParamsType, StateType, OutputType, LocalType>
{
public:
    FalconsLocalizationVision() {};
    ~FalconsLocalizationVision() {};

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
        return MRA::LoadDefaultParams<ParamsType>("components/falcons/localization_vision/interface/DefaultParams.json");
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

}; // class FalconsLocalizationVision


// component name
inline std::string componentName()
{
    return "FalconsLocalizationVision";
}


// configuration handling
inline ParamsType defaultParams()
{
    return FalconsLocalizationVision().defaultParams();
}
inline ParamsType loadParams(std::string configFile)
{
    return MRA::LoadDefaultParams<ParamsType>(configFile);
}

} // namespace MRA::FalconsLocalizationVision

#endif

