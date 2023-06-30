#ifndef _MRA_BASE_ABSTRACT_INTERFACE_HPP
#define _MRA_BASE_ABSTRACT_INTERFACE_HPP

#include <google/protobuf/util/time_util.h>

// provide a better implementation of google::protobuf::util::TimeUtil::GetCurrentTime()
// because it lacks sub-second resolution (until it was reluctantly fixed in newer versions)
// https://github.com/protocolbuffers/protobuf/issues/8930
inline google::protobuf::Timestamp GetCurrentTime()
{
    google::protobuf::Timestamp result;
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(now);
    result.set_seconds(secs.count());
    result.set_nanos(std::chrono::duration_cast<std::chrono::nanoseconds>(now - secs).count());
    return result;
}

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

