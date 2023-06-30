#ifndef _MRA_LIBRARIES_LOGGING_HPP
#define _MRA_LIBRARIES_LOGGING_HPP

#include "abstract_interface.hpp"

namespace MRA::Logging
{

typedef google::protobuf::Timestamp Tt;

// template for use in scoped tick logging
// see also logger ideas and requirements in https://github.com/janfeitsma/MRA-prototype/issues/10
// TODO: find a way to log component name, probably need macro
// TODO: there can be large (binary) data present, which probably should be automatically filtered, at least when dumping to stdout
template <typename Ti, typename Tp, typename Ts, typename To, typename Tl>
class LogTick
{
public:
    LogTick(Tt const &timestamp, Ti const &input, Tp const &params, Ts *state, To *output, Tl *local, int *error_value)
    :
        // store data for inspection later
        _t0(google::protobuf::util::TimeUtil::GetCurrentTime()),
        _state(state),
        _output(output),
        _local(local),
        _err(error_value)
    {
        // logging at START of the tick
        std::cout << "tick " << _counter << " START" << std::endl;
        std::cout << "   timestamp: " << timestamp << std::endl;
        std::cout << "   input: " << convert_proto_to_json_str(input) << std::endl;
        std::cout << "   params: " << convert_proto_to_json_str(params) << std::endl;
        //std::cout << "   state: " << convert_proto_to_json_str(*_state) << std::endl;
    }
    ~LogTick()
    {
        // logging at END of the tick
        double duration = 1e-6 * google::protobuf::util::TimeUtil::DurationToMicroseconds(google::protobuf::util::TimeUtil::GetCurrentTime() - _t0);
        std::cout << "tick " << _counter++ << " END, error_value=" << *_err << std::endl;
        std::cout << "   duration: " << duration << std::endl;
        std::cout << "   output: " << convert_proto_to_json_str(*_output) << std::endl;
        //std::cout << "   state: " << convert_proto_to_json_str(*_state) << std::endl;
        //std::cout << "   local: " << convert_proto_to_json_str(*_local) << std::endl;
    }
    
private:
    // store data for logging at destruction (when tick ends, the logged object goes out of scope)
    Tt   _t0;
    Ts  *_state;
    To  *_output;
    Tl  *_local;
    int *_err;
    static int _counter;

}; // template class LogTick

// initialize the static counter
template <typename Ti, typename Tp, typename Ts, typename To, typename Tl>
int LogTick<Ti, Tp, Ts, To, Tl>::_counter = 0;

} // namespace MRA::Logging

#endif // #ifndef _MRA_LIBRARIES_LOGGING_HPP

