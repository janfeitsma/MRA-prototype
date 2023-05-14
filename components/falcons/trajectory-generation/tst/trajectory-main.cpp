#include "FalconsTrajectoryGeneration.hpp"


int run(
    MRA::FalconsTrajectoryGeneration::InputType &input,
    MRA::FalconsTrajectoryGeneration::OutputType &output)
{
    auto m = MRA::FalconsTrajectoryGeneration::FalconsTrajectoryGeneration();

    std::cout << "input: " << MRA::convert_proto_to_json_str(input) << std::endl;
    int error_value = m.tick(input, output);
    
    if (error_value)
    {
        std::cerr << "ERROR: model returned error_value " << error_value << std::endl;
    }
    else
    {
        std::cout << "output: " << MRA::convert_proto_to_json_str(output) << std::endl;
    }
    
    // TODO: move table printer here, put all samples on output [repeated]?

    return error_value;
}


int main(int argc, char **argv)
{
    // To prevent depending on boost, option handling is handled by python wrapper -> see trajectory.py.
    // User should not directly use this binary.
    if (argc < 3)
    {
        std::cerr << "insufficient arguments" << std::endl;
        return 1;
    }
    std::string input_str = argv[1];
    std::string param_str = argv[2]; // VelocityControl params

    // TODO: TrajectoryGeneration params? dt, maxticks, ...

    // Setup the inputs
    auto input = MRA::FalconsTrajectoryGeneration::Input();
    auto output = MRA::FalconsTrajectoryGeneration::Output();

    // Apply provided json strings
    assert(google::protobuf::util::JsonStringToMessage(input_str, &input).ok());
    //assert(google::protobuf::util::JsonStringToMessage(param_str, &input));
    input.mutable_worldstate()->mutable_robot()->set_active(true);

    // Run and display
    return run(input, output);
}

