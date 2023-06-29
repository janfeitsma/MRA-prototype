#include "opencv_utils.hpp"



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

