#include "fit.hpp"

// MRA libraries
#include "opencv_utils.hpp"

using namespace MRA::FalconsLocalizationVision;


FitAlgorithm::FitAlgorithm()
{
}

FitAlgorithm::~FitAlgorithm()
{
}

FitResult FitAlgorithm::run(cv::Mat const &referenceFloor, cv::Mat const &rcsLinePoints)
{
    FitResult result;

    // create solver
    auto cvSolver = cv::DownhillSolver::create();

    // configure solver
    cv::Ptr<FitFunction> f = new FitFunction(referenceFloor, rcsLinePoints);
    cvSolver->setFunction(f);
    cv::Mat step = (cv::Mat_<double>(3, 1) << 50, 50, 40); // TODO make configurable
    cvSolver->setInitStep(step);
	cv::TermCriteria criteria = cvSolver->getTermCriteria();
	criteria.maxCount = 400; // TODO make configurable
	cvSolver->setTermCriteria(criteria);

    // set initial guess
    cv::Mat vec = (cv::Mat_<double>(1, 3) << settings.guess.x, settings.guess.y, settings.guess.rz);

    // run solver, result will be in 'vec'
    result.score = cvSolver->minimize(vec);

    // store result
    result.success = true;
    result.pose.x = vec.at<double>(0, 0);
    result.pose.y = vec.at<double>(0, 1);
    result.pose.rz = vec.at<double>(0, 2);
    result.floor = f->getFloor();
    return result;
}

FitFunction::FitFunction(cv::Mat const &referenceFloor, cv::Mat const &rcsLinePoints)
{
    _referenceFloor = referenceFloor;
    _rcsLinePoints = rcsLinePoints;
    _lastFloor = MRA::OpenCVUtils::joinWhitePixels(_referenceFloor, _rcsLinePoints);
}

double FitFunction::calc(const double *x) const
{
    // create a transformation matrix
    cv::Mat transformationMatrix = cv::getRotationMatrix2D(cv::Point2f(_rcsLinePoints.cols/2, _rcsLinePoints.rows/2), x[2], 1.0);
    transformationMatrix.at<double>(0, 2) += x[0];
    transformationMatrix.at<double>(1, 2) += x[1];
    // apply the transformation to _rcsLinePoints
    cv::Mat transformedLinePoints;
    cv::warpAffine(_rcsLinePoints, transformedLinePoints, transformationMatrix, _rcsLinePoints.size());
    // calculate the intersection of the white pixels using bitwise AND operation
    cv::Mat overlapMask;
    cv::bitwise_and(_referenceFloor, transformedLinePoints, overlapMask);
    // calculate score
    double overlapScore = static_cast<double>(countNonZero(overlapMask)); // / (_rcsLinePoints.rows * _rcsLinePoints.cols);
    printf("FitFunction::calc (%7.3f,%7.3f,%7.3f) -> score %7.3f\n", x[0], x[1], x[2], overlapScore);
    return overlapScore;
}

