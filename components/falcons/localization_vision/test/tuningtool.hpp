#ifndef TUNINGTOOL_HPP
#define TUNINGTOOL_HPP

#include <opencv2/opencv.hpp>
#include <google/protobuf/message.h>

#include "tuningparam.hpp"

class TuningTool
{
public:
    TuningTool(const google::protobuf::Message& message);

    void run();

    void setValue(const std::string& key, int value); // int because of opencv slider design

    template <typename T>
    T getValue(const std::string& paramName) const
    {
        checkParamExists(paramName);
        if (std::is_same<T, int>::value)
        {
            return 1; //return parameters[paramName]->value;
        }
        return 1.0; //static_cast<T>(parameters[paramName]->value);
    }

    int getTBValue(const std::string& paramName) const // TODO remove?
    {
        checkParamExists(paramName);
        int result = parameters.at(paramName)->value;
        return result;
    }

    int getTBCount(const std::string& paramName) const
    {
        checkParamExists(paramName);
        return 100;
    }

    template <typename T>
    void setParameterMaxValue(const std::string& paramName, T value)
    {
        checkParamExists(paramName);
        //if (std::is_same<decltype(value), float())) 
        if (std::is_same<T, float>::value) 
        {
            parameters[paramName]->maxValue = static_cast<int>(value);
        }
        //if (std::is_same<T, decltype(int())>)
        //{
        //    parameters[paramName]->maxValue = value;
        //}
    }

private:
    cv::Mat generateImage(); // TODO make externally configurable

    const google::protobuf::Message& protobufMessage;

    std::unordered_map<std::string, std::shared_ptr<TunableParameter>> parameters;

    void checkParamExists(const std::string& paramName) const;

};

#endif  // TUNINGTOOL_HPP

