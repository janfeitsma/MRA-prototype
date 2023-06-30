#ifndef _MRA_LIBRARIES_OPENCV_UTILS_HPP
#define _MRA_LIBRARIES_OPENCV_UTILS_HPP

#include "datatypes/CvMat.pb.h"
#include <opencv2/opencv.hpp>

namespace MRA::OpenCVUtils
{

void serializeCvMat(cv::Mat const &src, MRA::Datatypes::CvMatProto &tgt);
void deserializeCvMat(MRA::Datatypes::CvMatProto const &src, cv::Mat &tgt);

cv::Mat joinWhitePixels(const cv::Mat& mat1, const cv::Mat& mat2);

} // namespace MRA::OpenCVUtils

#endif // #ifndef _MRA_LIBRARIES_OPENCV_UTILS_HPP

