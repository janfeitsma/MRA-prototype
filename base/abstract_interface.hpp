#ifndef _MRA_BASE_ABSTRACT_INTERFACE_HPP
#define _MRA_BASE_ABSTRACT_INTERFACE_HPP

#include <google/protobuf/util/time_util.h>


namespace MRA
{

template <typename T_InputType, typename T_ParamsType, typename T_StateType, typename T_OutputType, typename T_LocalType>
class MRAInterface
{
public:
    using InputType = T_InputType;
    using ParamsType = T_ParamsType;
    using StateType = T_StateType;
    using OutputType = T_OutputType;
    using LocalType = T_LocalType;

    MRAInterface() {};
    ~MRAInterface() {};

    virtual int tick(
        google::protobuf::Timestamp timestamp,   // absolute timestamp
        InputType  const           &input,       // input data, type generated from Input.proto
		ParamsType const           &params,      // configuration parameters, type generated from Params.proto
		StateType                  &state,       // state data, type generated from State.proto
		OutputType                 &output,      // output data, type generated from Output.proto
		LocalType                  &local        // local/diagnostics data, type generated from Local.proto
    ) = 0;

}; // template class MRAInterface

} // namespace MRA

#endif // _MRA_BASE_ABSTRACT_INTERFACE_HPP
