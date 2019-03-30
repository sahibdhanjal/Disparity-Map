# Disparity Map Calculator - C++ Sample:
The given template function calculates the disparity map given 2 stereo images. We follow the algorithm [here](https://sites.google.com/site/5kk73gpu2010/assignments/stereo-vision#TOC-Update-Disparity-Map) because of ease of implementation and understanding. We use OpenCV 3.4.2 for this project and it is built on a linux system.

## Result:
The result obtained by running ```./build/disparity``` is [disparity_map.png](disparity_map.png)

## Build and Run:
- The repository is provided with a prebuilt binary ```./build/disparity```
- To rebuild the code, proceed as follows:
    - goto ```build``` folder using ```cd build/```
    - remove prebuilt binary using ```rm disparity```
    - parse cmake lists using ```cmake ..```
    - make target using ```make```
- The new binary can be used as follows:
    - **goto the root directory**
    - ```./build/disparity <image_left> <image_right> <max_disparity> <kernel_radius>```
    - all the arguments are optional
    - the binary uses the images in the ```images/``` folder by default with a **max disparity** of **64** and **kernel radius** of **4**

## Class Interface
The interface for the ```Disparity``` class looks as follows:
```
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

```

Two constants are defined:
- ```K_SIZE = 4```, which is the radius of the kernel which convolving the image. Hence, the kernel size is ```2*K_SIZE + 1```, as it should always be odd
- ```D_MAX``` = 64 , which is the maximum disparity checked for
