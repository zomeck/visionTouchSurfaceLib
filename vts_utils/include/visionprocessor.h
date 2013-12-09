#ifndef VISIONPROCESSOR_H
#define VISIONPROCESSOR_H

#include "kinecthandler.h"
#include "projectivemapping.h"
#include "fourierdescriptor.h"

#include "framedata.h"

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

    int wResolution,hResolution;

    vector<Point2f> screenContoursCorners;


    virtual int segmentScreen(const Mat & grayFrame, int Ith,float epsilon, vector<Point2i> & screenContours, Mat & maskShow );
public:
    VisionProcessor();

    virtual void init( int wRes=1024, int hRes=768 );
    virtual void getScreenTransformation();

    virtual void createMaxDepthWithHistogram( int numFrames=200 );

    virtual void processFrame( FrameData & frameData, double & processingDuration );

    cv::Size2i getKinectResolution() const;

    ProjectiveMapping getKinect2screen() const;
    void setKinect2screen(const ProjectiveMapping &value);

};

#endif // VISIONPROCESSOR_H
