// this code has been mostly produced by free chatGPT (v3.5?), quite amazing

#include "tuningtool.hpp"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/reflection.h>
#include <google/protobuf/dynamic_message.h>

#define IMAGE_WINDOW_NAME "Image"
#define SLIDER_WINDOW_NAME "Tuning sliders"
#define MAX_NUM_SLIDERS 18
#define FREQUENCY 10.0

class TunableParameter {
public:
    std::string name;
    google::protobuf::FieldDescriptor::Type type;
    int minValue;
    int maxValue;
    int defaultValue;
    int value;

    TunableParameter(const std::string& n, google::protobuf::FieldDescriptor::Type t, int min, int max, int def)
        : name(n), type(t), minValue(min), maxValue(max), defaultValue(def), value(def) {}
};

class TunableFloatParameter : public TunableParameter {
public:
    TunableFloatParameter(const std::string& n, int min, int max, float def)
        : TunableParameter(n, google::protobuf::FieldDescriptor::TYPE_FLOAT, min, max, static_cast<int>(def)) {}

    float getFloatValue() const {
        return static_cast<float>(value);
    }
};

class TunableIntParameter : public TunableParameter {
public:
    TunableIntParameter(const std::string& n, int min, int max, int def)
        : TunableParameter(n, google::protobuf::FieldDescriptor::TYPE_INT32, min, max, def) {}
};

std::vector<TunableParameter*> createTunableParameters(const google::protobuf::Message& message, const std::string& prefix = "");

void createTrackbars(TuningTool* tuningTool, const std::vector<TunableParameter*>& parameters);

TuningTool::TuningTool(const google::protobuf::Message& message) : protobufMessage(message) {}

void TuningTool::setValue(const std::string& parameterName, int value)
{
    sliderValues[parameterName] = value;
    // Handle the updated value as needed
}

void TuningTool::run()
{
    cv::namedWindow(IMAGE_WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::namedWindow(SLIDER_WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::resizeWindow(SLIDER_WINDOW_NAME, 200, 150);
    std::vector<TunableParameter*> tunableParameters = createTunableParameters(protobufMessage);
    if (tunableParameters.size() > MAX_NUM_SLIDERS)
    {
        std::cerr << "Warning: Too many sliders, dropping some - TODO generate more slider windows?" << std::endl;
        tunableParameters.resize(MAX_NUM_SLIDERS);
    }
    createTrackbars(this, tunableParameters);

    while (true)
    {
        cv::Mat image = generateImage();
        cv::imshow(IMAGE_WINDOW_NAME, image);
        int key = cv::waitKey(1000 / FREQUENCY);
        if (key == 'q')
            break;
    }
    cv::destroyAllWindows();
}

std::vector<TunableParameter*> createTunableParameters(const google::protobuf::Message& message, const std::string& prefix)
{
    std::vector<TunableParameter*> tunableParameters;

    const google::protobuf::Descriptor* descriptor = message.GetDescriptor();
    const google::protobuf::Reflection* reflection = message.GetReflection();

    if (descriptor->options().map_entry())
    {
        std::cerr << "Warning: Skipping map entry field \"" << descriptor->name() << "\"." << std::endl;
        return tunableParameters;
    }

    for (int i = 0; i < descriptor->field_count(); i++)
    {
        const google::protobuf::FieldDescriptor* field = descriptor->field(i);
        std::string fieldName = prefix + field->name();

        if (field->is_repeated())
        {
            std::cerr << "Warning: Skipping repeated field \"" << fieldName << "\"." << std::endl;
            continue;
        }

        int defaultValue = 0;

        if (field->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE)
        {
            // recurse
            const google::protobuf::Message& nestedMessage = reflection->GetMessage(message, field);
            std::vector<TunableParameter *> nestedParameters = createTunableParameters(nestedMessage, fieldName + ".");
            tunableParameters.insert(tunableParameters.end(), nestedParameters.begin(), nestedParameters.end());
        }
        else if (reflection->HasField(message, field))
        {
            if (field->type() == google::protobuf::FieldDescriptor::TYPE_FLOAT ||
                field->type() == google::protobuf::FieldDescriptor::TYPE_DOUBLE)
            {
                defaultValue = static_cast<int>(reflection->GetFloat(message, field));
                TunableFloatParameter* parameter = new TunableFloatParameter(fieldName, 0, 100, defaultValue);
                tunableParameters.push_back(parameter);
            }
            else if (field->type() == google::protobuf::FieldDescriptor::TYPE_INT32 ||
                     field->type() == google::protobuf::FieldDescriptor::TYPE_INT64)
            {
                defaultValue = reflection->GetInt32(message, field);
                TunableIntParameter* parameter = new TunableIntParameter(fieldName, 0, 100, defaultValue);
                tunableParameters.push_back(parameter);
            }
            else if (field->type() == google::protobuf::FieldDescriptor::TYPE_BOOL)
            {
                defaultValue = reflection->GetBool(message, field);
                TunableIntParameter* parameter = new TunableIntParameter(fieldName, 0, 1, defaultValue);
                tunableParameters.push_back(parameter);
            }
        }
    }

    return tunableParameters;
}

void onTrackbarChange(int value, void* userData)
{
    std::pair<TuningTool*, std::string>* data = static_cast<std::pair<TuningTool*, std::string>*>(userData);
    TuningTool* tuningTool = data->first;
    std::string parameterName = data->second;

    tuningTool->setValue(parameterName, value);
}

void createTrackbars(TuningTool* tuningTool, const std::vector<TunableParameter*>& parameters)
{
    for (const TunableParameter* parameter : parameters)
    {
        int defaultValue = parameter->defaultValue;

        cv::createTrackbar(parameter->name, SLIDER_WINDOW_NAME, &tuningTool->sliderValues[parameter->name], parameter->maxValue, onTrackbarChange,
            new std::pair<TuningTool*, std::string>(tuningTool, parameter->name));
        cv::setTrackbarPos(parameter->name, SLIDER_WINDOW_NAME, defaultValue);
    }
}

// below is temporary mock-up, to be moved outside and connected with algorithm

cv::Mat TuningTool::generateImage()
{
    // Generate the cv::Mat object based on the slider values
    // Implement your custom logic here based on the slider values
    // Example: Generate a simple 200x200 image with BGR color channels based on two sliders
    int value1 = sliderValues["actionRadius.x"];
    int value2 = sliderValues["actionRadius.y"];
    cv::Mat image(200, 200, CV_8UC3, cv::Scalar(value1, value2, 0));

    return image;
}

