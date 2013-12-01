#ifndef LEAPHANDLER_H
#define LEAPHANDLER_H

#include <iostream>

#include <opencv2/opencv.hpp>
#include <highgui.h>

using namespace std;
using namespace cv;

#include "Leap.h"

#include "leapmapping.h"

class LeapHandler
{

    Leap::Frame frame;

public:
    LeapHandler();
    bool determineTouch(Leap::Vector position);

    virtual void waitFrame();

    virtual bool findBestMatchOnCurrentFrame( Point & inputWorld, float radius, LeapMapping & leap2world, Point3i &match );

    float depthThresh;
    Leap::Controller leapSensor;

};

#endif // LEAPHANDLER_H

