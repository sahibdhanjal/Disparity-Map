# Disparity Map Calculator - C++ Sample:
The given template function calculates the disparity map given 2 stereo images.

## Build and Run:
- The repository is provided with a prebuilt binary ```./build/disparity```
- To rebuild the code, proceed as follows:
    - goto ```build``` folder using ```cd build/```
    - remove prebuilt binary using ```rm disparity```
    - parse cmake lists using ```cmake ..```
    - make target using ```make```
- The new binary can be used as follows:
    - goto the root directory
    - ```./build/disparity <image_left> <image_right> <max_disparity>```
    - all the arguments are optional and you can have the following variations:
        - ```./build/disparity```
        - ```./build/disparity <image_left> <image_right>```
        - ```./build/disparity <image_left> <image_right> <max_disparity>```
    - the binary used the images in the ```images/``` folder by default with a max disparity of 64

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
    cv::Mat fr_left, fr_right, disp_map, disp_map_color;
    int fr_width, fr_height, max_disp;

    // preprocesses the frame before disparity calculation
    void preprocessFrame(cv::Mat& frame);
    // calculates the disparity map
    void calcDisparityMap();

public:
    // constructor and destructor
    Disparity(cv::Mat left, cv::Mat right, int disp);
    ~Disparity();

    // sets left and right frames to new frames
    void setFrames(cv::Mat left, cv::Mat right);
    // outputs disparity map
    cv::Mat getDisparityMap();
};
```
