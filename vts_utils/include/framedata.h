#ifndef FRAMEDATA_H
#define FRAMEDATA_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <highgui.h>

#include "projectivemapping.h"
#include "leapmapping.h"

#include "hand.h"
using namespace std;
using namespace cv;

class FrameData
{
public:
    FrameData();

    //vector< vector<Point> > * contourHandsFiltered;
    //vector< vector< vector< Point > > > *contoursFingersHands;
    //vector< bool > *foundPalmHands;
    vector<Hand> *hands;
    ProjectiveMapping * kinect2screen;
    LeapMapping * leap2screen;
    Point3i matchPointable;
    Mat depthMap;
    Mat rgbDebug;
};

#endif // FRAMEDATA_H
