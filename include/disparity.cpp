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
    cv::Mat result = cv::Mat(frame.size(), CV_64FC3);
    cv::filter2D(frame, result, -1, kernel, cv::Point(-1,-1), 0.0, cv::BORDER_CONSTANT);
    return result;
}

// algorithm proceeds as per the following tutorial
// https://sites.google.com/site/5kk73gpu2010/assignments/stereo-vision#TOC-Update-Disparity-Map
void Disparity::calcDisparityMap() {

    // define kernel with appropriate size
    int size_ = 2*KERNEL_RADIUS + 1; double value_ = 1.0/double(size_);
    std::vector<std::vector<double>> kernel_( size_, std::vector<double> (size_, value_) );
    cv::Mat kernel(size_, size_, CV_32F, &kernel_);

    // calculate SAD for all levels of shift
    std::vector<cv::Mat> disp_maps(this->max_disp);
    for(int k = 0; k < this->max_disp; k++) {
        cv::Mat r_shifted = Disparity::shift(this->fr_right, 100);
        cv::Mat r_diff; cv::absdiff(this->fr_left, r_shifted, r_diff);
        cv::Mat r_filtered = Disparity::imfilter(r_diff, kernel);
        disp_maps[k] = r_filtered;
    }

    // calculate final disparity map by calculating minimums
    this->disp_map = cv::Mat::ones(this->fr_left.size(), CV_64FC3)*(double)(2.0);
    for(int row = 0; row < this->fr_left.rows; row++) {
        for(int col = 0 ; col < this->fr_left.cols; col++) {
            printf("(%d,%d) : %f\n", row, col, this->disp_map.at<double>(row,col));
        //     for(int k = 0; k < this->max_disp ; k++)
        //         MIN = std::min(MIN, disp_maps[k].at<double>(row,col));
        //     this->disp_map.at<double>(row,col) = MIN;
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
        Disparity::calcDisparityMap();
    }
    return this->disp_map;
}
