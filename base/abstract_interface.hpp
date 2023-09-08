#ifndef _MRA_BASE_ABSTRACT_INTERFACE_HPP
#define _MRA_BASE_ABSTRACT_INTERFACE_HPP

#include <google/protobuf/util/time_util.h>


namespace MRA
{

template <typename Ti, typename Tp, typename Ts, typename To, typename Tl>
class MRAInterface
{
public:

    using InputType = Ti;
    using ParamsType = Tp;
    using StateType = Ts;
    using OutputType = To;
    using LocalType = Tl;

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

