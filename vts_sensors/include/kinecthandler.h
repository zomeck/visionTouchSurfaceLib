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

class KinectHandler
{
private:

    Context         context;
    ImageGenerator      g_image;
    DepthGenerator      d_image;
    ImageMetaData       g_imageMD;
    EnumerationErrors   errors;
    XnStatus        nRetVal;
    const XnDepthPixel* pDepthMap;
    const XnRGB24Pixel* pImage;

    int ResWidth,ResHeight;

protected:

    //virtual void findKeypointsUsingBlobs(Mat & touch, vector<cv::KeyPoint> & keypoints,int minArea, int maxArea );

public:
    KinectHandler();
    ~KinectHandler();
    virtual void init();
    virtual void waitFrame();
    virtual void getDepthMap16( Mat & depthMap );
    virtual void getDepthMap08( Mat & depthMap );
    virtual void getImage( Mat & image );
    static void depthMapCorrection(Mat & input, Mat & output , bool interPolate = false);

    int getResWidth() const;
    void setResWidth(int value);
    int getResHeight() const;
    void setResHeight(int value);
};

#endif // KinectHandler_H
