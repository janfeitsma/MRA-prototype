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
        _t(timestamp),
        _t0(GetCurrentTime()),
        _input(input),
        _params(params),
        _state(state),
        _output(output),
        _local(local),
        _err(error_value),
        _filename(std::string("/tmp/tick") + std::to_string(_counter) + ".bin") // TODO include component name
    {
        start();
    }

    void start()
    {
        // logging at START of the tick
        startStdout();
        if (_dump) startDumpToFile();
    }

    void startStdout()
    {
        std::cout << "tick " << _counter << " START" << std::endl;
        std::cout << "   timestamp: " << _t << std::endl;
        std::cout << "   input: " << convert_proto_to_json_str(_input) << std::endl;
        std::cout << "   params: " << convert_proto_to_json_str(_params) << std::endl;
        //std::cout << "   state: " << convert_proto_to_json_str(*_state) << std::endl;
    }

    ~LogTick()
    {
        end();
    }

    void end()
    {
        // logging at END of the tick
        endStdout();
        if (_dump) endDumpToFile();
        _counter++;
    }

    void endStdout()
    {
        double duration = 1e-6 * google::protobuf::util::TimeUtil::DurationToMicroseconds(GetCurrentTime() - _t0);
        std::cout << "tick " << _counter << " END, error_value=" << *_err << std::endl;
        std::cout << "   duration: " << duration << std::endl;
        std::cout << "   output: " << convert_proto_to_json_str(*_output) << std::endl;
        //std::cout << "   state: " << convert_proto_to_json_str(*_state) << std::endl;
        //std::cout << "   local: " << convert_proto_to_json_str(*_local) << std::endl;
    }

    template <typename T>
    void dumpToFile(T const &pbObject, std::ofstream *fp)
    {
        if (!fp || !fp->is_open()) return;

        // serialize the protobuf object to a string
        std::ostringstream oss;
        pbObject.SerializeToOstream(&oss);
        std::string serializedData = oss.str();

        // write the byte count followed by the serialized object
        int byteCount = static_cast<int>(serializedData.size());
        fp->write(reinterpret_cast<const char*>(&byteCount), sizeof(int));
        fp->write(serializedData.c_str(), byteCount);
    }

    void startDumpToFile()
    {
        _filepointer = new std::ofstream();
        _filepointer->open(_filename);
        dumpToFile(_input, _filepointer);
        dumpToFile(_params, _filepointer);
        dumpToFile(*_state, _filepointer);
    }

    void endDumpToFile()
    {
        dumpToFile(*_output, _filepointer);
        dumpToFile(*_local, _filepointer);
        dumpToFile(*_state, _filepointer);
        _filepointer->close();
        delete _filepointer;
    }

private:
    // store data for logging at destruction (when tick ends, the logged object goes out of scope)
    Tt         _t0;
    Tt         _t;
    Ti const  &_input;
    Tp const  &_params;
    Ts        *_state;
    To        *_output;
    Tl        *_local;
    int       *_err;
    static int _counter;
    bool       _dump = true; // TODO improve configurability
    std::string _filename;
    std::ofstream *_filepointer = NULL;

}; // template class LogTick

// initialize the static counter
template <typename Ti, typename Tp, typename Ts, typename To, typename Tl>
int LogTick<Ti, Tp, Ts, To, Tl>::_counter = 0;

} // namespace MRA::Logging

#endif // #ifndef _MRA_LIBRARIES_LOGGING_HPP

