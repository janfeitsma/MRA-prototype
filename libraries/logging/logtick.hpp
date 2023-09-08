#ifndef _MRA_LIBRARIES_LOGGING_LOGTICK_HPP
#define _MRA_LIBRARIES_LOGGING_LOGTICK_HPP

#include "abstract_interface.hpp"
#include "backend.hpp"
#include "control.hpp"
#include <fstream>


namespace MRA::Logging
{

typedef google::protobuf::Timestamp Tt;

// template for use in scoped tick logging
// see also logger ideas and requirements in https://github.com/janfeitsma/MRA-prototype/issues/10
// TODO: there can be large (binary) data present, which probably should be automatically filtered, at least when dumping to stdout
template <typename Ti, typename Tp, typename Ts, typename To, typename Tl>
class LogTick
{
public:
    LogTick(std::string componentName, std::string fileName, int lineNumber, Tt const &timestamp, Ti const &input, Tp const &params, Ts *state, To *output, Tl *local, int *error_value)
    :
        // store data for inspection later
        _componentName(componentName),
        _fileName(fileName),
        _lineNumber(lineNumber),
        _t(timestamp),
        _t0(google::protobuf::util::TimeUtil::GetCurrentTime()),
        _input(input),
        _params(params),
        _state(state),
        _output(output),
        _local(local),
        _err(error_value)
    {
        start();
    }

    ~LogTick()
    {
        end();
    }

    void start()
    {
        // get configuration to use for this tick (do not allow logging only start or only end of tick)
        _cfg = control::getConfiguration(_componentName);
        // dispatch to backend
        if (_cfg.enabled())
        {
            backend::reconfigure(_cfg);
            // if so configured, open binary file, otherwise NULL pointer
            _binfile = backend::logTickBinFile(_cfg, _componentName, _counter);
            // call backend
            backend::logTickStart(_componentName, _fileName, _lineNumber, _cfg, _binfile, _counter, _t, _input, _params, *_state);
        }
    }

    void end()
    {
        // dispatch to backend
        if (_cfg.enabled())
        {
            // calculate tick duration
            auto elapsed = google::protobuf::util::TimeUtil::GetCurrentTime() - _t0;
            double duration_sec = 1e-9 * google::protobuf::util::TimeUtil::DurationToNanoseconds(elapsed);
            // call backend
            backend::logTickEnd(_componentName, _fileName, _lineNumber, _cfg, _binfile, _counter, duration_sec, *_err, *_state, *_output, *_local);
        }
        // update counter for next tick
        _counter++;
    }

private:
    // store data for logging at destruction (when tick ends, the logged object goes out of scope)
    Tt          _t0;
    Tt          _t;
    Ti const   &_input;
    Tp const   &_params;
    Ts         *_state;
    To         *_output;
    Tl         *_local;
    int        *_err;
    static int  _counter;
    std::string _componentName;
    std::string _fileName;
    int         _lineNumber;
    MRA::Datatypes::LogSpec _cfg;
    std::ofstream *_binfile = NULL;

}; // template class LogTick

// initialize the static counter
template <typename Ti, typename Tp, typename Ts, typename To, typename Tl>
int LogTick<Ti, Tp, Ts, To, Tl>::_counter = 0;

} // namespace MRA::Logging

#endif // #ifndef _MRA_LIBRARIES_LOGGING_LOGTICK_HPP


