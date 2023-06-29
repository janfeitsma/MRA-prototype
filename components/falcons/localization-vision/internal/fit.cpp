#include "fit.hpp"

using namespace MRA::FalconsLocalizationVision;

FitAlgorithm::FitAlgorithm()
{
}

FitAlgorithm::~FitAlgorithm()
{
}

FitResult FitAlgorithm::run(cv::Mat const &referenceFloor)
{
    FitResult result;

    // create solver
    auto cvSolver = cv::DownhillSolver::create();

    // configure solver
    cv::Ptr<FitFunction> f = new FitFunction();
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
    return result;
}

FitFunction::FitFunction()
{
}

double FitFunction::calc(const double *x) const
{
    // TODO
    return 0.0;
}

