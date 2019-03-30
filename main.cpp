#include "include/disparity.h"

int main(int argc, char** argv) {
    char *fr_left, *fr_right;
    int disp, k_size;

    // block to take console input
    if(argc<=5) {
        int nl = strlen("images/left.png");
        int nr = strlen("images/right.png");
        fr_left = new char[nl+1]; strcpy(fr_left, "images/left.png");
        fr_right = new char[nr+1]; strcpy(fr_right, "images/right.png");

        fr_left = (argc>=2) ? argv[1] : fr_left;
        fr_right = (argc>=3) ? argv[2] : fr_right;
        disp = (argc>=4) ? strtol(argv[3], NULL, 10) : D_MAX;
        k_size = (argc==5) ? strtol(argv[4], NULL, 10) : K_SIZE;
    }

    else {
        std::cout<<"Input :=> disparity <left_image> <right_image> <max_disparity> <kernel_radius>"<<std::endl;
        return -1;
    }

    // read images
    cv::Mat left = cv::imread(fr_left, CV_LOAD_IMAGE_COLOR);
    cv::Mat right = cv::imread(fr_right, CV_LOAD_IMAGE_COLOR);

    // return in case image can't be opened
    if( ! left.data || ! right.data ) {
        std::cerr<<"Could not open image"<<std::endl;
        return -1;
    }

    // make object of disparity map class. calculate and write image
    Disparity dmap(left, right, disp, k_size);
    cv::Mat disp_map = dmap.getDisparityMap();
    cv::namedWindow( "Disparity Map", cv::WINDOW_AUTOSIZE );
    cv::imshow( "Disparity Map", disp_map );
    cv::waitKey(0);

    return 0;
}
