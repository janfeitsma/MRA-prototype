/*
#include "FalconsLocalizationVision.hpp"
#include "tuningtool.hpp"



int main(int argc, char **argv)
{
    MRA::FalconsLocalizationVision::Params p = MRA::FalconsLocalizationVision::defaultParams();
    // TODO: option to call loadParams(filename)?

    // setup tuning tool
    // only solver settings, no sliders for field dimensions etc
    TuningTool t(p.solver());

    // set range hints, this is implementation specific ... TODO: somehow generate ranges from Params.proto?
    t.setParameterMaxValue("numExtraThreads", 10);
    t.setParameterMaxValue("pixelsPerMeter", 200);
    t.setParameterMaxValue("floorBorder", 2.0);
    t.setParameterMaxValue("blurFactor", 2.0);
    t.setParameterMaxValue("actionRadius.x", 3.0);
    t.setParameterMaxValue("actionRadius.y", 3.0);
    t.setParameterMaxValue("actionRadius.rz", 3.0);
    t.setParameterMaxValue("maxCount", 500);
    t.setParameterMaxValue("epsilon", 1e-3); // TODO logarithmic?
    //t.setParameterRange("linePoints.fit.radiusConstant", 0.0, 0.5);
    //t.setParameterRange("linePoints.fit.radiusScaleFactor", -0.01, 0.01);
    //t.setParameterRange("linePoints.fit.radiusMinimum", 0.0, 0.5);
    //t.setParameterRange("linePoints.plot.radius", 0.0, 0.5);
    t.setParameterMaxValue("random.count", 10);
    t.setParameterMaxValue("random.searchRadius", 10.0);
    t.setParameterMaxValue("random.exclusionRadius", 10.0);
    t.setParameterMaxValue("random.maxTries", 10);

    // run the tool
    t.run();

    return 0;
}
*/

/*
#include <opencv2/opencv.hpp>

// Define the desired range for the floating-point values
const float minValue = -1.0;
const float maxValue = 1.0;
const int numSteps = 100;  // Number of steps for mapping the range

float getFloatValue(int trackbarPosition)
{
    // Scale the trackbar position to the floating-point range
    float range = maxValue - minValue;
    float stepSize = range / numSteps;
    return minValue + (trackbarPosition * stepSize);
}

int getTrackbarPosition(float floatValue)
{
    // Map the floating-point value back to the trackbar position
    float range = maxValue - minValue;
    float stepSize = range / numSteps;
    return static_cast<int>((floatValue - minValue) / stepSize);
}

void onTrackbarrChange(int trackbarPosition, void* userData)
{
    // Convert the trackbar position to the corresponding floating-point value
    float floatValue = getFloatValue(trackbarPosition);

    // Handle the floating-point value as needed
    // ...
    std::cout << "floatValue " << floatValue << std::endl;
}

int main()
{
    // Create a window
    cv::namedWindow("Slider Window", cv::WINDOW_NORMAL);

    // Create a trackbar
    int initialTrackbarPosition = getTrackbarPosition(0.0);
    cv::createTrackbar("Float Trackbar", "Slider Window", &initialTrackbarPosition, numSteps, onTrackbarrChange);

    // Run the application loop
    cv::waitKey(0);

    return 0;
}
*/

#include <opencv2/opencv.hpp>

const float minValue = -1.0;
const float maxValue = 1.0;

float trackbarValue = 0.0;

void onTrackbarChange(int position, void* userData)
{
    // Convert the trackbar position to the corresponding float value
    float range = maxValue - minValue;
    float stepSize = range / 100;  // 100 steps
    trackbarValue = minValue + (position * stepSize);

    // Handle the float value as needed
    // ...
}

int main()
{
    cv::namedWindow("Slider Window", cv::WINDOW_NORMAL);

    // Create a trackbar with float values
    const char* trackbarName = "Float Trackbar";
    int numSteps = 100;
    cv::createTrackbar2(trackbarName, "Slider Window", nullptr, numSteps, onTrackbarChange);

    while (true)
    {
        // Display the current float value on the trackbar
        cv::setTrackbarPos(trackbarName, "Slider Window", static_cast<int>((trackbarValue - minValue) * numSteps / (maxValue - minValue)));

        // Handle other application logic
        // ...

        // Exit the loop on ESC key press
        if (cv::waitKey(1) == 27)
            break;
    }

    return 0;
}


