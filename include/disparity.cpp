#include "disparity.h"

Disparity::Disparity(cv::Mat left, cv::Mat right, int disp) {
    this->fr_left = left;
    this->fr_right = right;
    this->max_disp = disp;
}

Disparity::~Disparity() {
}

void Disparity::preprocessFrame(cv::Mat& frame) {
    /*
    * Preprocess for disparity calculation
    * - convert to grayscale
    * - equalize histogram
    * - gaussian blur
    */
    cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(frame, frame);
    cv::GaussianBlur(frame, frame, cv::Size(5,5), 0);
}

void Disparity::calcDisparityMap() {
    this->disp_map = this->fr_left;
}

void Disparity::setFrames(cv::Mat left, cv::Mat right) {
    this->fr_left = left;
    this->fr_right = right;
}

cv::Mat Disparity::getDisparityMap() {
    // error checking block
    if( ! this->fr_left.data || ! this->fr_right.data ) {
        std::cerr<<"Could not open image"<<std::endl;
    }
    else if( this->fr_left.cols!=this->fr_right.cols || this->fr_left.rows!=this->fr_right.rows ) {
        std::cerr<<"Images are not the same size"<<std::endl;
    }
    else {
        Disparity::preprocessFrame(this->fr_left);
        Disparity::preprocessFrame(this->fr_right);
        Disparity::calcDisparityMap();
    }
    return this->disp_map;
}
