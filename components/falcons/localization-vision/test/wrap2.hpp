#ifdef _MSC_VER
    #define EXPORT_SYMBOL __declspec(dllexport)
#else
    #define EXPORT_SYMBOL
#endif

#include "FalconsLocalizationVision.hpp"

#ifdef __cplusplus
extern "C" {
#endif

//namespace MRA::FalconsLocalizationVision
//{

EXPORT_SYMBOL int pywrap_tick_standalone(
        google::protobuf::Timestamp timestamp,   // absolute timestamp
        MRA::FalconsLocalizationVision::InputType  const           &input,       // input data, type generated from Input.proto
        MRA::FalconsLocalizationVision::ParamsType const           &params,      // configuration parameters, type generated from Params.proto
        MRA::FalconsLocalizationVision::StateType                  &state,       // state data, type generated from State.proto
        MRA::FalconsLocalizationVision::OutputType                 &output,      // output data, type generated from Output.proto
        MRA::FalconsLocalizationVision::LocalType                  &local        // local/diagnostics data, type generated from Local.proto
    );

//}
        
#ifdef __cplusplus
}
#endif
