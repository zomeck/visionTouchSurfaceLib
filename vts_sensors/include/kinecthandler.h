#ifndef KINECTHANDLER_H
#define KINECTHANDLER_H

#include <iostream>

#include <XnOpenNI.h>

#include <XnCppWrapper.h>

#include <opencv2/opencv.hpp>
#include <highgui.h>


using namespace std;
using namespace cv;
using namespace xn;

/**
 * @brief The KinectHandler class is an interface to comunicate with the kinect SDK
 *
 The aim of this class is being able to make the system more modular this class is the only one needed to be modified when there are changes on the kinect SDK
 */
class KinectHandler
{
private:

protected:
    Context         context;
    ImageGenerator      g_image;
    DepthGenerator      d_image;
    ImageMetaData       g_imageMD;
    EnumerationErrors   errors;
    XnStatus        nRetVal;
    const XnDepthPixel* pDepthMap;
    const XnRGB24Pixel* pImage;

    int ResWidth,ResHeight;



    //virtual void findKeypointsUsingBlobs(Mat & touch, vector<cv::KeyPoint> & keypoints,int minArea, int maxArea );

public:
    KinectHandler();
    ~KinectHandler();

    /**
     * @brief init creates the conection with the device
     */
    virtual void init();

    /**
     * @brief waitFrame waits until there is a frame avaliable to retrieve
     */
    virtual void waitFrame();

    /**
     * @brief getDepthMap16 retrieves the depthmap of the current frame 16 precision
     * @param depthMap i/o variable that returns the depthmap using a opencv Mat of type CV_16UC1
     */
    virtual void getDepthMap16( Mat & depthMap );

    /**
     * @brief getDepthMap16 retrieves the depthmap of the current frame 8 precision
     * @param depthMap i/o variable that returns the depthmap using a opencv Mat of type CV_8UC1
     */
    virtual void getDepthMap08( Mat & depthMap );

    /**
     * @brief getImage retrieves the RGB image from the current frame
     * @param image i/o variable that returns the RGB image using a opencv Mat of type CV_8UC3
     */
    virtual void getImage( Mat & image );

    /**
     * @brief depthMapCorrection uses opencv inpaint to correct the holes from the depthmap
     * @param input is an opencv Mat of type CV_16UC1 depthmap with holes to be corrected
     * @param output is an opencv Mat of type CV_8UC1 depthmap with corrected holes
     * @param interPolate if true the computing is interpolated for faster performance
     */
    static void depthMapCorrection(Mat & input, Mat & output , bool interPolate = false);

    int getResWidth() const;
    void setResWidth(int value);
    int getResHeight() const;
    void setResHeight(int value);
};

#endif // KinectHandler_H
