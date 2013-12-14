#ifndef VISIONPROCESSOR_H
#define VISIONPROCESSOR_H

#include "kinecthandler.h"
#include "projectivemapping.h"
#include "fourierdescriptor.h"

#include "framedata.h"

/**
 * @brief The VisionProcessor class performs image processing to the sensor data in order to detect the hands and the fingertips
 */
class VisionProcessor
{
protected:

    const static int HAND_CLASS_ID=1;
    KinectHandler kinect;
    Mat mask,maxDepth;
    ProjectiveMapping kinect2screen;
    float errorTolerance;
    float thicknessHand,thicknesFinger;
    int sizes[2];
    float tolerance;
    int epsilon;
    Mat minDepthHand,minDepthFinger;
    float minAreaContour,maxAreaContour;
    Mat erodeK;
    bool useScreenSeg;
    FourierDescriptor fdDictionary;
    static const double PI = 3.14159265358979323846;

    float maxAreaFinger;
    float minAreaFinger;
    float maxExcentricity;
    float minExcentricity;
    float maxCirc;
    float minHandExc;
    float maxDist2Class;
    int descriptorNum;

    int wResolution,hResolution;

    vector<Point2f> screenContoursCorners;


    virtual int segmentScreen(const Mat & grayFrame, int Ith,float epsilon, vector<Point2i> & screenContours, Mat & maskShow );
public:
    VisionProcessor();

    /**
     * @brief init initializes the paramameters for the image processing algorithms
     * @param wRes is the width of the resolution the screen is working on, 1024 for default
     * @param hRes is the height of the resolution the screen is working on, 768 for default
     */
    virtual void init( int wRes=1024, int hRes=768 );

    /**
     * @brief getScreenTransformation segments and creates the transformation for going from sensor space to screen space
     */
    virtual void getScreenTransformation();

    /**
     * @brief createMaxDepthWithHistogram generates a noise robust background map
     * @param numFrames number of frames to generate the histogram, 200 for default
     */
    virtual void createMaxDepthWithHistogram( int numFrames=200 );

    /**
     * @brief processFrame waits for a frame and does the processing
     * @param frameData input/output variable with the frame data processed
     * @param processingDuration output variable with processing time needed in seconds
     */
    virtual void processFrame( FrameData & frameData, double & processingDuration );

    cv::Size2i getKinectResolution() const;

    ProjectiveMapping getKinect2screen() const;
    void setKinect2screen(const ProjectiveMapping &value);

};

#endif // VISIONPROCESSOR_H
