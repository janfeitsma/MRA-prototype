#ifndef TUNINGTOOL_HPP
#define TUNINGTOOL_HPP

#include <opencv2/opencv.hpp>
#include <google/protobuf/message.h>

class TuningTool
{
public:
    TuningTool(const google::protobuf::Message& message);

    void run();

private:
    static void onSliderChange(int value, void* userData);

    int createSlidersForFields(const google::protobuf::Message& message, const std::string& prefix = "");

    cv::Mat generateImage();

    const google::protobuf::Message& protobufMessage;
    std::unordered_map<std::string, int> sliderValues;
};

#endif  // TUNINGTOOL_HPP

