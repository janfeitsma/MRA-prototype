#ifndef TUNINGTOOL_HPP
#define TUNINGTOOL_HPP

#include <opencv2/opencv.hpp>
#include <google/protobuf/message.h>

class TuningTool
{
public:
    TuningTool(const google::protobuf::Message& message);

    void run();

    void setValue(const std::string& key, int value); // int because of opencv slider design

    std::unordered_map<std::string, int> sliderValues; // TODO should be private?

private:
    cv::Mat generateImage(); // TODO make externally configurable

    const google::protobuf::Message& protobufMessage;
};

#endif  // TUNINGTOOL_HPP

