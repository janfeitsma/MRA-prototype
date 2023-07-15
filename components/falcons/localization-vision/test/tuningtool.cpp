// this code has been mostly produced by free chatGPT (v3.5?), quite amazing

#include "tuningtool.hpp"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/reflection.h>
#include <google/protobuf/dynamic_message.h>


#define IMAGE_WINDOW_NAME "Image"
#define SLIDER_WINDOW_NAME "Tuning sliders"
#define MAX_NUM_SLIDERS 10
#define FREQUENCY 10.0


TuningTool::TuningTool(const google::protobuf::Message& message) : protobufMessage(message) {}

void TuningTool::run()
{
    cv::namedWindow(IMAGE_WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::namedWindow(SLIDER_WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::resizeWindow(SLIDER_WINDOW_NAME, 700, 200);
    int n = createSlidersForFields(protobufMessage);
    std::cout << "Number of sliders: " << n << std::endl;
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

void TuningTool::onSliderChange(int value, void* userData)
{
    std::pair<TuningTool*, std::string>* data = static_cast<std::pair<TuningTool*, std::string>*>(userData);
    TuningTool* tuningTool = data->first;
    std::string sliderName = data->second;
    tuningTool->sliderValues[sliderName] = value;
}

int TuningTool::createSlidersForFields(const google::protobuf::Message& message, const std::string& prefix)
{
    int result = 0;
    const google::protobuf::Descriptor* descriptor = message.GetDescriptor();
    const google::protobuf::Reflection* reflection = message.GetReflection();

    if (descriptor->options().map_entry())
    {
        std::cerr << "Warning: Skipping map entry field \"" << descriptor->name() << "\"." << std::endl;
        return result;
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

        if (field->type() == google::protobuf::FieldDescriptor::TYPE_FLOAT ||
            field->type() == google::protobuf::FieldDescriptor::TYPE_DOUBLE ||
            field->type() == google::protobuf::FieldDescriptor::TYPE_INT32 ||
            field->type() == google::protobuf::FieldDescriptor::TYPE_INT64)
        {
            cv::createTrackbar(fieldName, SLIDER_WINDOW_NAME, &sliderValues[fieldName], 100, onSliderChange, new std::pair<TuningTool*, std::string>(this, fieldName));
            result++;

            // Set the minimum and maximum values for the slider (modify as per your protobuf options)
            // TODO, maybe range hints, would be cool if these could be generated from Params.proto?
            cv::setTrackbarMin(fieldName, SLIDER_WINDOW_NAME, 0);
            cv::setTrackbarMax(fieldName, SLIDER_WINDOW_NAME, 100);

            // Set default value
            if (reflection->HasField(message, field))
            {
                if (field->type() == google::protobuf::FieldDescriptor::TYPE_FLOAT)
                {
                    cv::setTrackbarPos(fieldName, SLIDER_WINDOW_NAME, reflection->GetFloat(message, field));
                }
                else if (field->type() == google::protobuf::FieldDescriptor::TYPE_DOUBLE)
                {
                    cv::setTrackbarPos(fieldName, SLIDER_WINDOW_NAME, reflection->GetDouble(message, field));
                }
                else if (field->type() == google::protobuf::FieldDescriptor::TYPE_INT32)
                {
                    cv::setTrackbarPos(fieldName, SLIDER_WINDOW_NAME, reflection->GetInt32(message, field));
                }
                else if (field->type() == google::protobuf::FieldDescriptor::TYPE_INT64)
                {
                    cv::setTrackbarPos(fieldName, SLIDER_WINDOW_NAME, reflection->GetInt64(message, field));
                }
            }

            // not supported in opencv4
            //cv::setTrackbarHeight(fieldName, SLIDER_WINDOW_NAME, TRACKBAR_HEIGHT);
            //cv::setTrackbarProperty(fieldName, SLIDER_WINDOW_NAME, cv::WIDGET_POSITION, cv::WIDGET_POSITION_LEFT);
        }
        else if (field->type() == google::protobuf::FieldDescriptor::TYPE_BOOL)
        {
            bool defaultValue = false;
            if (reflection->HasField(message, field))
                defaultValue = reflection->GetBool(message, field);

            cv::createTrackbar(fieldName, SLIDER_WINDOW_NAME, &sliderValues[fieldName], 1, onSliderChange, new std::pair<TuningTool*, std::string>(this, fieldName));
            result++;
            cv::setTrackbarPos(fieldName, SLIDER_WINDOW_NAME, defaultValue ? 1 : 0);

            // not supported in opencv4
            //cv::setTrackbarHeight(fieldName, SLIDER_WINDOW_NAME, TRACKBAR_HEIGHT);
            //cv::setTrackbarProperty(fieldName, SLIDER_WINDOW_NAME, cv::WIDGET_POSITION, cv::WIDGET_POSITION_LEFT);
        }
        else if (field->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE)
        {
            const google::protobuf::Message& nestedMessage = reflection->GetMessage(message, field);
            result += createSlidersForFields(nestedMessage, fieldName + ".");
        }
    }
    return result;
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

