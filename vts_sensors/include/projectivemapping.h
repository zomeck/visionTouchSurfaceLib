#ifndef PROJECTIVEMAPPING_H
#define PROJECTIVEMAPPING_H

#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


class ProjectiveMapping
{
public:
    ProjectiveMapping();
    ProjectiveMapping(int width,int height);
    ProjectiveMapping( const ProjectiveMapping & param );
    virtual void calculateMapping(const vector<Point2f> &quadrangle_corners);
    virtual void mapPoints(vector<Point2f> &points, vector<Point2f> &mappedPoints);
    virtual void mapPoints(Point2i &point, Point2i &mappedPoint);
    virtual void warpMatrix(Mat &matrix, Mat &warpedMatrix, Size &warpedSize);

protected:
    Mat transformationMatrix;

private:
    vector<Point2f> rect_corners;

};

#endif // PROJECTIVEMAPPING_H

