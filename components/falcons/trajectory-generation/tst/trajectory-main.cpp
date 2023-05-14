#include "FalconsTrajectoryGeneration.hpp"


int run(
    MRA::FalconsTrajectoryGeneration::InputType &input,
    MRA::FalconsTrajectoryGeneration::ParamsType &params,
    MRA::FalconsTrajectoryGeneration::OutputType &output)
{
    auto m = MRA::FalconsTrajectoryGeneration::FalconsTrajectoryGeneration();

    int error_value = m.tick(input, params, output);
    
    if (error_value)
    {
        std::cerr << "ERROR: model returned error_value " << error_value << std::endl;
    }
    else
    {
        // output as one json object, for processing by trajectory.py
        std::cout << "{" << std::endl;
        std::cout << "\"input\": " << MRA::convert_proto_to_json_str(input) << "," << std::endl;
        std::cout << "\"params\": " << MRA::convert_proto_to_json_str(params) << "," << std::endl;
        std::cout << "\"output\": " << MRA::convert_proto_to_json_str(output) << std::endl;
        std::cout << "}" << std::endl;
    }

    return error_value;
}


int main(int argc, char **argv)
{
    // To prevent depending on boost, option handling is handled by python wrapper -> see trajectory.py.
    // User should not directly use this binary.
    // The python script should also take care of pretty-printing the sample data, and/or plotting.
    if (argc < 3)
    {
        std::cerr << "insufficient arguments" << std::endl;
        return 1;
    }
    std::string input_str = argv[1];
    std::string param_str = argv[2]; // VelocityControl params are nested

    // Setup the inputs
    auto input = MRA::FalconsTrajectoryGeneration::Input();
    auto params = MRA::FalconsTrajectoryGeneration::Params();
    auto output = MRA::FalconsTrajectoryGeneration::Output();

    // Apply provided json strings
    assert(google::protobuf::util::JsonStringToMessage(input_str, &input).ok());
    assert(google::protobuf::util::JsonStringToMessage(param_str, &params).ok());
    input.mutable_worldstate()->mutable_robot()->set_active(true); // for convenience

    // Run and display actual jsons
    return run(input, params, output);
}

