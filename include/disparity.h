#ifndef DISPARITY_H
#define DISPARITY_H

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core_c.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>

#define K_SIZE 4    // window radius of kernel
#define D_MAX 64    // maximum disparity

class Disparity {
private:
    /*
    allocate variables for storing the left and right frames,
    the frames in B&W and color after calculating disparity,
    and integer dimensions of each frame and the max disparity
    */
    cv::Mat fr_left, fr_right;
    int max_disp, k_size;

    // preprocesses the frame before disparity calculation
    void preprocessFrame(cv::Mat& frame);
    // shifts frame by x,y
    cv::Mat shift(cv::Mat& frame, int x, int y);
    // function replicates the functionality of imfilter from MATLAB
    cv::Mat imfilter(cv::Mat& frame, cv::Mat& kernel);
    // calculates the disparity map
    cv::Mat calcDisparityMap();

public:
    // constructor and destructor
    Disparity(cv::Mat left, cv::Mat right, int disp, int k_rad);
    ~Disparity();

    // sets left and right frames to new frames
    void setFrames(cv::Mat left, cv::Mat right);
    // outputs disparity map
    cv::Mat getDisparityMap();

};

#endif
