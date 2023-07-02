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

    void configure(Params const &config);

    cv::Mat createMat();
    void letterModelToShapes(StandardLetterModel const &model, std::vector<MRA::Datatypes::Shape> &shapes);
    void shapesToCvMat(std::vector<MRA::Datatypes::Shape> const &shapes, float blurFactor, cv::Mat &m);
    void linePointsToCvMat(std::vector<Landmark> const &linePoints, cv::Mat &m, float overruleRadius = 0.0);
    cv::Point pointFcsToPixel(MRA::Datatypes::Point const &p) const;

    // diagnostics-specific
    void addGridLines(cv::Mat &m, float step, cv::Scalar color) const;

private:
    Params settings;
    float _ppm = 1;
    float _sizeX;
    float _sizeY;
    float _originX;
    float _originY;
    int _numPixelsX;
    int _numPixelsY;

}; // class Floor

} // namespace MRA::FalconsLocalizationVision

#endif

