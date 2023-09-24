#include "fit.hpp"

// MRA libraries
#include "opencv_utils.hpp"
#include "logging.hpp"

using namespace MRA::FalconsLocalizationVision;


void FitAlgorithm::run(cv::Mat const &referenceFloor, std::vector<cv::Point2f> const &rcsLinePoints, std::vector<Tracker> &trackers)
{
    MRA_TRACE_FUNCTION();
    // TODO: multithreading

    // run all fit attempts
    for (auto &tr: trackers)
    {
        FitResult fr = _fitCore.run(referenceFloor, rcsLinePoints, tr.guess, tr.step);
        tr.fitResult = fr.pose;
        tr.fitValid = fr.valid;
        tr.fitScore = fr.score;
    }

    // sort trackers on decreasing quality
    std::sort(trackers.begin(), trackers.end());
}

FitResult FitCore::run(cv::Mat const &referenceFloor, std::vector<cv::Point2f> const &rcsLinePoints, MRA::Geometry::Pose const &guess, MRA::Geometry::Pose const &step)
{
    int numpoints = rcsLinePoints.size();
    MRA::Datatypes::Pose guess_ = guess;
    MRA::Datatypes::Pose step_ = step;
    MRA_TRACE_FUNCTION_INPUTS(numpoints, guess_, step_);
    FitResult result;

    // create solver
    auto cvSolver = cv::DownhillSolver::create();

    // configure solver
    cv::Ptr<FitFunction> f = new FitFunction(referenceFloor, rcsLinePoints, settings.pixelspermeter());
    cvSolver->setFunction(f);
    cv::Mat stepVec = (cv::Mat_<double>(3, 1) << step.x, step.y, step.rz);
    cvSolver->setInitStep(stepVec);
	cv::TermCriteria criteria = cvSolver->getTermCriteria();
	criteria.maxCount = settings.maxcount();
	criteria.epsilon = settings.epsilon();
	cvSolver->setTermCriteria(criteria);

    // set initial guess
    cv::Mat vec = (cv::Mat_<double>(1, 3) << guess.x, guess.y, guess.rz);

    // run solver, result will be in 'vec'
    result.score = cvSolver->minimize(vec);

    // store result
    result.valid = true; // TODO score threshold
    result.pose.x = (vec.at<double>(0, 0));
    result.pose.y = (vec.at<double>(0, 1));
    result.pose.rz = (vec.at<double>(0, 2));
    return result;
}

FitFunction::FitFunction(cv::Mat const &referenceFloor, std::vector<cv::Point2f> const &rcsLinePoints, float ppm)
{
    MRA_TRACE_FUNCTION();
    _ppm = ppm;
    _referenceFloor = referenceFloor;
    _rcsLinePoints = rcsLinePoints;
    // count pixels for normalization, prevent division by zero when no linepoints present (yet)
    _rcsLinePointsPixelCount = rcsLinePoints.size();
    MRA_TRACE_FUNCTION_OUTPUT(_rcsLinePointsPixelCount);
}

double FitFunction::calcOverlap(cv::Mat const &m1, cv::Mat const &m2) const
{
    MRA_TRACE_FUNCTION();
    // calculate the intersection of the white pixels using bitwise AND operation
    cv::Mat overlapMask;
    cv::bitwise_and(m1, m2, overlapMask);
    // calculate score, normalize on the value stored at construction time
    // (which runtime is passed as m2 after transformation)
    double result = static_cast<double>(countNonZero(overlapMask)) / _rcsLinePointsPixelCount;
    // clip score & confidence into [0.0, 1.0]
    result = std::max(0.0, std::min(1.0, result));
    MRA_TRACE_FUNCTION_OUTPUT(result);
    return result;
}

cv::Mat FitFunction::transform3dof(cv::Mat const &m, double x, double y, double rz) const
{
    MRA_TRACE_FUNCTION();
    cv::Mat result;
    // create a transformation matrix
    float rad2deg = 180.0 / M_PI;
    cv::Mat transformationMatrix = cv::getRotationMatrix2D(cv::Point2f(0.5 * m.cols, 0.5 * m.rows), rz * rad2deg, 1.0);
    transformationMatrix.at<double>(0, 2) += y * _ppm; // flip xy, cv::Mat is landscape mode
    transformationMatrix.at<double>(1, 2) += x * _ppm; // flip xy, cv::Mat is landscape mode
    // apply the transformation
    cv::warpAffine(m, result, transformationMatrix, m.size());
    return result;
}

std::vector<cv::Point2f> FitFunction::transform3dof(const std::vector<cv::Point2f> &points, double x, double y, double rz) const
{
    MRA_TRACE_FUNCTION();
    // Nothing to do?
    if (points.size() == 0)
    {
        return points;
    }
    std::vector<cv::Point2f> transformedPoints;
    float rad2deg = 180.0 / M_PI;
    // Construct the transformation matrix
    cv::Mat transformationMatrix = cv::getRotationMatrix2D(cv::Point2f(), rz * rad2deg, 1.0);
    transformationMatrix.at<double>(0, 2) = x;
    transformationMatrix.at<double>(1, 2) = y;
    // Transform each point
    cv::transform(points, transformedPoints, transformationMatrix);
    return transformedPoints;
}

double FitFunction::calc(const double *v) const
{
    double x = v[0];
    double y = v[1];
    double rz = v[2];
    MRA_TRACE_FUNCTION_INPUTS(x, y, rz);
    double score = 0.0;
    std::vector<cv::Point2f> transformed = transform3dof(_rcsLinePoints, v[0], v[1], v[2]);
    for (size_t i = 0; i < _rcsLinePoints.size(); ++i)
    {
        int pixelX = static_cast<int>(transformed[i].x * _ppm);
        int pixelY = static_cast<int>(transformed[i].y * _ppm);
        float s = 0.0;
        // Check if the pixel is within the image bounds
        if (pixelX >= 0 && pixelX < _referenceFloor.cols && pixelY >= 0 && pixelY < _referenceFloor.rows)
        {
            // Look up pixel intensity in _referenceFloor and accumulate the score
            s = static_cast<float>(_referenceFloor.at<uchar>(pixelY, pixelX)) / 255.0;
            score += s; // max 1.0 per pixel
        }
        MRA_LOG_DEBUG("calc %3d   ix=%8.3f  iy=%8.3f  tx=%8.3f  ty=%8.3f  px=%4d py=%4d  s=%6.2f\n", (int)i, _rcsLinePoints[i].x, _rcsLinePoints[i].y, transformed[i].x, transformed[i].y, (int)(pixelX), (int)(pixelY), s);
    }
    // final normalization to 0..1 where 0 is good (minimization)
    double result = 1.0 - score / _rcsLinePointsPixelCount;
    MRA_TRACE_FUNCTION_OUTPUT(result);
    return result;
}

