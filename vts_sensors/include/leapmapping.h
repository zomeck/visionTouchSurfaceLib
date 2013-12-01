#ifndef LEAPMAPPING_H
#define LEAPMAPPING_H

#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


class LeapMapping
{
public:
    LeapMapping();
    LeapMapping(int widthPx,int heightPx, float widthMM, float heightMM );
    virtual void calculateMapping(const Point &translationVecPx);
    virtual void mapPoints(vector<Point> &points, vector<Point> &mappedPoints);
    virtual void mapPoints(Point &point, Point &mappedPoint);
    //virtual void warpMatrix(Mat &matrix, Mat &warpedMatrix, Size &warpedSize);


protected:
    float numPxperMM_width;
    float numPxperMM_height;
    Mat transformationMatrix;

};

#endif // LEAPMAPPING_H
