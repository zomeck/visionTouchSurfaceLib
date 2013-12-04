#ifndef FINGER_H
#define FINGER_H

#include<vector>
#include <opencv2/opencv.hpp>
#include <highgui.h>


using namespace std;
using namespace cv;

class Finger
{
public:
    Finger();
    vector<Point> contour;
};

#endif // FINGER_H
