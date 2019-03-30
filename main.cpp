#include "include/disparity.h"

int main(int argc, char** argv) {
    char *fr_left, *fr_right;
    int max_disparity;

    // block to take console input
    if(argc==1) {
        int nl = strlen("images/left.png");
        int nr = strlen("images/right.png");
        fr_left = new char[nl+1]; strcpy(fr_left, "images/left.png");
        fr_right = new char[nr+1]; strcpy(fr_right, "images/right.png");
        max_disparity = 64;
    }

    else if (argc==3) {
        fr_left = argv[1]; fr_right = argv[2]; max_disparity = 64;
    }

    else if (argc==4) {
        fr_left = argv[1]; fr_right =argv[2]; max_disparity = strtol(argv[3], NULL, 10);
    }

    else {
        std::cerr<<"Input Format: disparity <left_image> <right_image> <max_disparity>"<<std::endl;
        return -1;
    }

    // read images
    cv::Mat left = cv::imread(fr_left, CV_LOAD_IMAGE_COLOR);
    cv::Mat right = cv::imread(fr_right, CV_LOAD_IMAGE_COLOR);

    // make object of disparity map class. calculate and write image
    Disparity dmap(left, right, max_disparity);
    cv::Mat disp_map = dmap.getDisparityMap();
    cv::namedWindow( "Disparity Map", cv::WINDOW_AUTOSIZE );
    cv::imshow( "Disparity Map", disp_map );
    cv::waitKey(0);

    return 0;
}
