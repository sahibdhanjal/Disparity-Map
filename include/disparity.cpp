#include "disparity.h"

// Constructor
Disparity::Disparity(cv::Mat left, cv::Mat right, int disp) {
    this->fr_left = left;
    this->fr_right = right;
    this->max_disp = disp;
}

// Default Destructor
Disparity::~Disparity() {
}

/*
* Preprocess for disparity calculation
* - convert to grayscale
* - equalize histogram
* - convert to grayscale
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
    cv::Mat result = cv::Mat(frame.size(), frame.type());
    cv::Point anchor(0,0); double delta = 0; int depth = -1;
    cv::filter2D(frame, result, depth, kernel, anchor, cv::BORDER_CONSTANT, cv::BORDER_CONSTANT);
    return result;
}

// algorithm proceeds as per the following tutorial
// https://sites.google.com/site/5kk73gpu2010/assignments/stereo-vision#TOC-Update-Disparity-Map
void Disparity::calcDisparityMap() {

    // define kernel with appropriate size
    int size_ = 2*KERNEL_RADIUS + 1; double value_ = 1.0/double(size_);
    std::vector<std::vector<double>> kernel_( size_, std::vector<double> (size_, value_) );
    cv::Mat kernel(size_, size_, CV_64FC3, &kernel_);

    // calculate SAD for all levels of shift
    std::vector<cv::Mat> disp_maps(this->max_disp);
    for(int k = 0; k < this->max_disp; k++) {
        cv::Mat shifted_left = Disparity::shift(this->fr_left, k);
        cv::Mat diffImage; cv::absdiff(shifted_left, this->fr_right, diffImage);
        cv::Mat filtered_left = Disparity::imfilter(diffImage, kernel);
        disp_maps[k] = filtered_left;
    }

    // calculate final disparity map by calculating minimums
    this->disp_map = cv::Mat(this->fr_left.size(), CV_32FC1);
    for(int row = 0; row < this->fr_left.rows; row++) {
        for(int col = 0 ; col < this->fr_left.cols; col++) {
            double MIN = 2;
            for(int k = 0; k < 1 ; k++)
                MIN = std::min(MIN, disp_maps[k].at<double>(row,col));
            this->disp_map.at<double>(row,col) = MIN;
        }
    }
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
        // Disparity::calcDisparityMap();
    }
    return this->disp_map;
}
