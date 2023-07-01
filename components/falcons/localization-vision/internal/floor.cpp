#include "floor.hpp"

using namespace MRA::FalconsLocalizationVision;

Floor::Floor()
{
}

Floor::~Floor()
{
}

void Floor::configure(Params const &config)
{
    settings.CopyFrom(config);
    _ppm = settings.solver().pixelspermeter();
    // determine floor size
    _sizeX = settings.model().b() + 2.0 * settings.solver().floorborder();
    _sizeY = settings.model().a() + 2.0 * settings.solver().floorborder();
    // origin is the FCS point at pixel (0,0)
    _originX = -0.5 * _sizeX;
    _originY = -0.5 * _sizeY;
    // number of pixels (rotated)
    _numPixelsY = int(_sizeY * _ppm);
    _numPixelsX = int(_sizeX * _ppm);
}

cv::Mat Floor::createMat()
{
    return cv::Mat::zeros(_numPixelsX, _numPixelsY, CV_8UC1);
}

void Floor::letterModelToShapes(StandardLetterModel const &model, std::vector<MRA::Datatypes::Shape> &shapes)
{
    // all coordinates are in FCS - conversion to pixels (and potential 90degree rotation) happens at to cv::Mat operations
    MRA::Datatypes::Shape s;
    s.set_linewidth(model.k());
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
        }
        else if (s.has_circle())
        {
            int radius = s.circle().radius() * _ppm;
            auto p = pointFcsToPixel(s.circle().center());
            cv::circle(m, p, radius, color, -1);
        }
    }
}

void Floor::linePointsToCvMat(std::vector<Landmark> const &linePoints, cv::Mat &m)
{
    // for every linepoint, create a circle
    std::vector<MRA::Datatypes::Shape> shapes;
    MRA::Datatypes::Shape s;
    for (auto const &p: linePoints)
    {
        float r = settings.solver().linepointradiusconstant();
        float sf = settings.solver().linepointradiusscalefactor();
        if (sf)
        {
            float distance = sqrt(p.x() * p.x() + p.y() * p.y());
            r += sf * distance;
        }
        s.mutable_circle()->mutable_center()->set_x(p.x());
        s.mutable_circle()->mutable_center()->set_y(p.y());
        s.mutable_circle()->set_radius(r); // in meters, not pixels (anymore)
        shapes.push_back(s);
    }
    // make use of shapesToCvMat
    shapesToCvMat(shapes, 0.0, m);
}

