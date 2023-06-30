#include "opencv_utils.hpp"


using namespace cv;


void MRA::OpenCVUtils::serializeCvMat(cv::Mat const &src, MRA::Datatypes::CvMatProto &tgt)
{
    tgt.set_width(src.cols);
    tgt.set_height(src.rows);
    tgt.set_type(src.type());
    const size_t dataSize = src.total() * src.elemSize();
    tgt.set_data(src.data, dataSize);
}

void MRA::OpenCVUtils::deserializeCvMat(MRA::Datatypes::CvMatProto const &src, cv::Mat &tgt)
{
    tgt.release();
    tgt.create(src.height(), src.width(), src.type());
    const std::string& data = src.data();
    memcpy(tgt.data, data.data(), data.size());
}

cv::Mat MRA::OpenCVUtils::joinWhitePixels(const cv::Mat& mat1, const cv::Mat& mat2)
{
    CV_Assert(mat1.channels() == 1 && mat2.channels() == 1);

    // Create a binary mask from the white pixels in the first Mat
    Mat whiteMask1;
    threshold(mat1, whiteMask1, 1, 255, THRESH_BINARY);

    // Create a binary mask from the white pixels in the second Mat
    Mat whiteMask2;
    threshold(mat2, whiteMask2, 1, 255, THRESH_BINARY);

    // Perform a bitwise OR operation to join the white pixels
    Mat result;
    bitwise_or(whiteMask1, whiteMask2, result);

    return result;
}

