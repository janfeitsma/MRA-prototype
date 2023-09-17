// this file was produced by MRA-codegen.py from template_tick.cpp
// with the intent of letting user add the implementation here

// generated component header:
#include "FalconsTestMraLogger.hpp"

using namespace MRA;

// custom includes, if any
// ...


int FalconsTestMraLogger::FalconsTestMraLogger::tick
(
    google::protobuf::Timestamp timestamp,   // absolute timestamp
    InputType  const           &input,       // input data, type generated from Input.proto
    ParamsType const           &params,      // configuration parameters, type generated from Params.proto
    StateType                  &state,       // state data, type generated from State.proto
    OutputType                 &output,      // output data, type generated from Output.proto
    LocalType                  &local        // local/diagnostics data, type generated from Local.proto
)
{
	int error_value = 0;
	MRA_LOG_TICK();

    // user implementation goes here

    float tts = input.timetosleep();
    if (tts > 0) {
        usleep(int(1e6 * tts));
    }

    if (input.generatecriticalmessage()) {
        MRA_LOG_CRITICAL("test critical %d", 37);
    }

    if (input.generateerrormessage()) {
        MRA_LOG_ERROR("test error %d", 38);
    }

    if (input.generatewarningmessage()) {
        MRA_LOG_WARNING("test warning %d", 39);
    }

    if (input.generateinfomessage()) {
        MRA_LOG_INFO("test info %d", 40);
    }

    if (input.generatedebugmessage()) {
        MRA_LOG_DEBUG("test debug %d", 41);
    }

    return error_value;
}

