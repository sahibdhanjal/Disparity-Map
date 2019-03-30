#include "disparity.h"

// Constructor
Disparity::Disparity(cv::Mat left, cv::Mat right, int disp, int k_rad) {
    this->fr_left = left;
    this->fr_right = right;
    this->max_disp = disp;
    this->k_size = k_rad;
}

// Default Destructor
Disparity::~Disparity() {
}

/*
* Preprocess for disparity calculation
* - convert to grayscale
* - equalize histogram
* - convert to dtype - double type
* - gaussian blur
*/
void Disparity::preprocessFrame(cv::Mat& frame) {
    cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(frame, frame);
    frame.convertTo(frame, CV_64FC3, 1.0/255.0);
    cv::GaussianBlur(frame, frame, cv::Size(5,5), 0);
}

// Shifts image by x,y towards right and bottom
cv::Mat Disparity::shift(cv::Mat& frame, int x = 0, int y = 0) {
    int rows = frame.rows, cols = frame.cols;
    cv::Mat out = cv::Mat::zeros(frame.size(), frame.type());
    frame(cv::Rect(0,0, frame.cols-x,frame.rows-y)).copyTo(out(cv::Rect(x,y,frame.cols-x,frame.rows-y)));
    return out;
}

cv::Mat Disparity::imfilter(cv::Mat& frame, cv::Mat& kernel) {
    cv::Mat result = cv::Mat::zeros(frame.size(), frame.type());
    cv::filter2D(frame, result, -1, kernel, cv::Point(-1,-1), 0.0, cv::BORDER_CONSTANT);
    return result;
}

// algorithm proceeds as per the following tutorial
// https://sites.google.com/site/5kk73gpu2010/assignments/stereo-vision#TOC-Update-Disparity-Map
cv::Mat Disparity::calcDisparityMap() {

    // define kernel with appropriate size
    int size_ = 2*this->k_size + 1;
    cv::Mat kernel = cv::Mat::ones( size_, size_, CV_32F )/ (float)(size_);

    // calculate SAD for all levels of shift
    std::vector<cv::Mat> maps_(this->max_disp);
    for(int k = 1; k <= this->max_disp; k++) {
        cv::Mat r_shifted = Disparity::shift(this->fr_right, k);
        cv::Mat r_diff; cv::absdiff(this->fr_left, r_shifted, r_diff);
        cv::Mat r_filtered = Disparity::imfilter(r_diff, kernel);
        maps_[k-1] = r_filtered;
    }

    // calculate final disparity map by calculating minimums
    cv::Mat result = this->fr_left;

    double max_val = (double)INT_MIN;
    for(int row = 0; row < this->fr_left.rows; row++) {
        for(int col = 0 ; col < this->fr_left.cols; col++) {
            double min_val = (double)INT_MAX;
            for(int k = 0; k < this->max_disp ; k++)
                min_val = std::min(min_val, maps_[k].at<double>(row,col));
            result.at<double>(row,col) = min_val;
            max_val = std::max(min_val, max_val);
        }
    }
    return result;
}

void Disparity::setFrames(cv::Mat left, cv::Mat right) {
    this->fr_left = left;
    this->fr_right = right;
}

cv::Mat Disparity::getDisparityMap() {
    // error checking block
    if( this->fr_left.cols!=this->fr_right.cols || this->fr_left.rows!=this->fr_right.rows )
        throw std::invalid_argument("Images are not the same size");
    else {
        Disparity::preprocessFrame(this->fr_left);
        Disparity::preprocessFrame(this->fr_right);
        return Disparity::calcDisparityMap();
    }
}
