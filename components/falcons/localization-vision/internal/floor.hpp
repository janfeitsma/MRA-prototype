#ifndef _MRA_FALCONS_LOCALIZATION_VISION_FLOOR_HPP
#define _MRA_FALCONS_LOCALIZATION_VISION_FLOOR_HPP

#include <opencv2/opencv.hpp>
#include "datatypes.hpp"


namespace MRA::FalconsLocalizationVision
{

class Floor
{
public:
    Floor();
    ~Floor();

    void configure(float sizeX, float sizeY, float pixelsPerMeter);

    cv::Mat createMat();
    void letterModelToShapes(StandardLetterModel const &model, std::vector<MRA::Datatypes::Shape> &shapes);
    void shapesToCvMat(std::vector<MRA::Datatypes::Shape> const &shapes, float blurFactor, cv::Mat &m);
    void linePointsToCvMat(std::vector<Landmark> const &linePoints, cv::Mat &m);

private:
    float _ppm = 1;
    float _originX;
    float _originY;
    int _numPixelsY;
    int _numPixelsX;

    cv::Point pointFcsToPixel(MRA::Datatypes::Point const &p) const;

}; // class Floor

} // namespace MRA::FalconsLocalizationVision

#endif

