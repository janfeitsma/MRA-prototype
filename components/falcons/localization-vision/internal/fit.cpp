#include "fit.hpp"

// MRA libraries
#include "opencv_utils.hpp"

using namespace MRA::FalconsLocalizationVision;


FitResult FitAlgorithm::run(cv::Mat const &referenceFloor, cv::Mat const &rcsLinePoints, MRA::Datatypes::Pose const &inputGuess, std::vector<GuessingSubParams> const &extraGuesses)
{
    std::vector<FitResult> results;

    // offset the input guess to ensure that it is included,
    // since by default the generate simplex would not hit it
    //     if the step is configured to be (sx,sy,srz) and initial guess is zero
    //     then the initial simplex evaluates at the following simplex:
    //         (-0.5*sx, -0.5*sy, -0.5*srz)
    //         ( 0.5*sx,       0,        0)
    //         (      0,  0.5*sy,        0)
    //         (      0,       0,  0.5*srz)

    MRA::Datatypes::Pose step = settings.actionradius();
    MRA::Datatypes::Pose g = inputGuess;
    g.set_rz(-0.5 * step.rz() + g.rz());
    results.push_back(_fitCore.run(referenceFloor, rcsLinePoints, g, step));

    // if so configured, run more fit attempts
    for (auto const &gp: extraGuesses)
    {
        g.set_x(gp.x());
        g.set_y(gp.y());
        g.set_rz(0.0);
        step.set_x(gp.range());
        step.set_y(gp.range());
        step.set_rz(2 * M_PI);
        results.push_back(_fitCore.run(referenceFloor, rcsLinePoints, g, step));
    }

    // sort results on decreasing quality
    std::sort(results.begin(), results.end());
    return results.at(0);
}

FitResult FitCore::run(cv::Mat const &referenceFloor, cv::Mat const &rcsLinePoints, MRA::Datatypes::Pose const &guess, MRA::Datatypes::Pose const &step)
{
    FitResult result;

    // create solver
    auto cvSolver = cv::DownhillSolver::create();

    // configure solver
    cv::Ptr<FitFunction> f = new FitFunction(referenceFloor, rcsLinePoints, settings.pixelspermeter());
    cvSolver->setFunction(f);
    cv::Mat stepVec = (cv::Mat_<double>(3, 1) << step.x(), step.y(), step.rz());
    cvSolver->setInitStep(stepVec);
	cv::TermCriteria criteria = cvSolver->getTermCriteria();
	criteria.maxCount = settings.maxcount();
	criteria.epsilon = settings.epsilon();
	cvSolver->setTermCriteria(criteria);

    // set initial guess
    cv::Mat vec = (cv::Mat_<double>(1, 3) << guess.x(), guess.y(), guess.rz());

    // run solver, result will be in 'vec'
    result.score = cvSolver->minimize(vec);

    // store result
    result.success = true;
    result.pose.x = (vec.at<double>(0, 0));
    result.pose.y = (vec.at<double>(0, 1));
    result.pose.rz = (vec.at<double>(0, 2));
    return result;
}

FitFunction::FitFunction(cv::Mat const &referenceFloor, cv::Mat const &rcsLinePoints, float ppm)
{
    _ppm = ppm;
    _referenceFloor = referenceFloor;
    _rcsLinePoints = rcsLinePoints;
    // count pixels for normalization, prevent division by zero when no linepoints present (yet)
    _rcsLinePointsPixelCount = std::max(double(cv::countNonZero(_rcsLinePoints)), 1.0);
    printf("FitFunction _rcsLinePointsPixelCount %9.3f\n", _rcsLinePointsPixelCount);
}

double FitFunction::calcOverlap(cv::Mat const &m1, cv::Mat const &m2) const
{
    // calculate the intersection of the white pixels using bitwise AND operation
    cv::Mat overlapMask;
    cv::bitwise_and(m1, m2, overlapMask);
    // calculate score, normalize on the value stored at construction time
    // (which runtime is passed as m2 after transformation)
    double result = static_cast<double>(countNonZero(overlapMask)) / _rcsLinePointsPixelCount;
    // clip score & confidence into [0.0, 1.0]
    result = std::max(0.0, std::min(1.0, result));
    return result;
}

cv::Mat FitFunction::transform3dof(cv::Mat const &m, double x, double y, double rz) const
{
    cv::Mat result;
    // create a transformation matrix
    float rad2deg = 180.0 / M_PI;
    cv::Mat transformationMatrix = cv::getRotationMatrix2D(cv::Point2f(m.cols/2, m.rows/2), rz * rad2deg, 1.0);
    transformationMatrix.at<double>(0, 2) += y * _ppm; // flip xy, cv::Mat is landscape mode
    transformationMatrix.at<double>(1, 2) += x * _ppm; // flip xy, cv::Mat is landscape mode
    // apply the transformation
    cv::warpAffine(m, result, transformationMatrix, m.size());
    return result;
}

double FitFunction::calc(const double *x) const
{
    cv::Mat transformedLinePoints = transform3dof(_rcsLinePoints, x[0], x[1], x[2]);
    double overlapNormalized = calcOverlap(_referenceFloor, transformedLinePoints);
    double score = 1.0 - overlapNormalized;
    printf("FitFunction::calc (%9.5f,%9.5f,%9.5f) -> score %7.5f\n", x[0], x[1], x[2], score);
    fflush(stdout);
    return score;
}

