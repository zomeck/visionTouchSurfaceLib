#ifndef PROJECTIVEMAPPING_H
#define PROJECTIVEMAPPING_H

#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/**
 * @brief The ProjectiveMapping class converts the coordinate system from a quadrangle to a rectangle with origen at (0,0)
 */
class ProjectiveMapping
{
public:
    ProjectiveMapping();

    /**
     * @brief ProjectiveMapping constructor that sets the rectangle dimensions
     * @param width of the rectangle
     * @param height of the rectangle
     */
    ProjectiveMapping(int width,int height);
    ProjectiveMapping( const ProjectiveMapping & param );

    /**
     * @brief calculateMapping computes the transformation matrix from the space given to the rectangle
     * @param quadrangle_corners contains the quadrangle corners
     */
    virtual void calculateMapping(const vector<Point2f> &quadrangle_corners);

    /**
     * @brief mapPoints converts form quadrangle to rectangle
     * @param points input points in the quadrangle space
     * @param mappedPoints output points in rectangle space
     */
    virtual void mapPoints(vector<Point2f> &points, vector<Point2f> &mappedPoints);

    /**
     * @brief mapPoints converts form quadrangle to rectangle
     * @param points input point in the quadrangle space
     * @param mappedPoints output point in rectangle space
     */
    virtual void mapPoints(Point2i &point, Point2i &mappedPoint);

    /**
     * @brief warpMatrix transforms a Mat from quadrangle space to a rectangle sapce
     * @param matrix input Mat with the data to be warped
     * @param warpedMatrix output Mat with the warped data
     * @param warpedSize size of the warpedMatrix is going to have
     */
    virtual void warpMatrix(Mat &matrix, Mat &warpedMatrix, Size &warpedSize);

protected:
    Mat transformationMatrix;

private:
    vector<Point2f> rect_corners;

};

#endif // PROJECTIVEMAPPING_H

