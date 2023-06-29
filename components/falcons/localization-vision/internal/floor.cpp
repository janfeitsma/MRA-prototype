#include "floor.hpp"

using namespace MRA::FalconsLocalizationVision;

Floor::Floor(float sizeX, float sizeY, float pixelsPerMeter)
{
    _ppm = pixelsPerMeter;
    // origin is the FCS point at pixel (0,0)
    _originX = -0.5 * sizeX;
    _originY = -0.5 * sizeY;
}

Floor::~Floor()
{
}

void Floor::letterModelToShapes(StandardLetterModel const &model, std::vector<MRA::Datatypes::Shape> &shapes)
{
    // all coordinates are in FCS - conversion to pixels (and potential 90degree rotation) happens at to cv::Mat operations
    MRA::Datatypes::Shape s;
    s.set_linewidth(model.k());
    // hard checks
    if (model.a() < 1.0)
    {
        throw std::runtime_error("configuration missing or value too small for field dimension A");
    }
    if (model.b() < 1.0)
    {
        throw std::runtime_error("configuration missing or value too small for field dimension B");
    }
    s.mutable_line()->mutable_from()->set_x(-0.5 * model.b());
    s.mutable_line()->mutable_from()->set_y(-0.5 * model.a());
    s.mutable_line()->mutable_to()  ->set_x(-0.5 * model.b());
    s.mutable_line()->mutable_to()  ->set_y( 0.5 * model.a());
    shapes.push_back(s);
    s.mutable_line()->mutable_from()->set_x(-0.5 * model.b());
    s.mutable_line()->mutable_from()->set_y( 0.5 * model.a());
    s.mutable_line()->mutable_to()  ->set_x( 0.5 * model.b());
    s.mutable_line()->mutable_to()  ->set_y( 0.5 * model.a());
    shapes.push_back(s);
    s.mutable_line()->mutable_from()->set_x( 0.5 * model.b());
    s.mutable_line()->mutable_from()->set_y( 0.5 * model.a());
    s.mutable_line()->mutable_to()  ->set_x( 0.5 * model.b());
    s.mutable_line()->mutable_to()  ->set_y(-0.5 * model.a());
    shapes.push_back(s);
    s.mutable_line()->mutable_from()->set_x( 0.5 * model.b());
    s.mutable_line()->mutable_from()->set_y(-0.5 * model.a());
    s.mutable_line()->mutable_to()  ->set_x(-0.5 * model.b());
    s.mutable_line()->mutable_to()  ->set_y(-0.5 * model.a());
    shapes.push_back(s);
}

cv::Point Floor::pointFcsToPixel(MRA::Datatypes::Point const &p) const
{
    // flip x,y so that the image better fits on a typical computer screen
    return cv::Point((p.y() - _originY) * _ppm, (p.x() - _originX) * _ppm);
}

void Floor::shapesToCvMat(std::vector<MRA::Datatypes::Shape> const &shapes, float blurFactor, cv::Mat &m)
{
    cv::Scalar color(255, 255, 255); // white
    for (auto const &s: shapes)
    {
        if (s.has_line())
        {
            int lw = s.linewidth() * _ppm;
            auto pfrom = pointFcsToPixel(s.line().from());
            auto pto = pointFcsToPixel(s.line().to());
            cv::line(m, pfrom, pto, color, lw);
            printf("line pfrom(%d,%d) pto(%d,%d)\n", pfrom.x, pfrom.y, pto.x, pto.y);
        }
    }
}

void Floor::serializeCvMat(cv::Mat const &m, CvMatProto *result)
{
    result->set_width(m.cols);
    result->set_height(m.rows);
    result->set_type(m.type());
    // Copy the cv::Mat data into a Protobuf bytes field
    const size_t dataSize = m.total() * m.elemSize();
    result->set_data(m.data, dataSize);
}

