#ifndef HAND_H
#define HAND_H

#include<vector>
#include <opencv2/opencv.hpp>
#include <highgui.h>

using namespace std;
using namespace cv;

#include "finger.h"

class Hand
{
public:
    Hand();

    vector<Finger> fingers;
    vector<Point> contour;
    Point2f centroid;
    int shapeType;
    bool foundPalm;


};

#endif // HAND_H
