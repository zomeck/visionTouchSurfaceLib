#include "leapmapping.h"

LeapMapping::LeapMapping()
{
}
LeapMapping::LeapMapping(int widthPx, int heightPx, float widthMM, float heightMM)
{
    numPxperMM_width=widthPx/widthMM;
    numPxperMM_height=heightPx/heightMM;
}

void LeapMapping::calculateMapping(const Point &translationVecPx)
{

    transformationMatrix = Mat(2,3,CV_32F);
    setIdentity(transformationMatrix);
    transformationMatrix.at<float>(0,2)=translationVecPx.x;
    transformationMatrix.at<float>(1,2)=translationVecPx.y;
    cout<<transformationMatrix<<endl;

}

void LeapMapping::mapPoints(vector<Point> &points, vector<Point> &mappedPoints)
{
    mappedPoints.clear();
    for(int i;i<points.size();i++)
    {

        Point point=points[i];
        Point mappedPoint;

        this->mapPoints(point,mappedPoint);
        mappedPoints.push_back(mappedPoint);

    }

}

void LeapMapping::mapPoints(Point &point, Point &mappedPoint)
{
    Mat pointH(3,1,CV_32F);
    pointH.at<float>(0)=point.x*numPxperMM_width;
    pointH.at<float>(1)=point.y*numPxperMM_height;
    pointH.at<float>(2)=1;

    Mat mappedPointH=transformationMatrix*pointH;

    mappedPoint = Point(mappedPointH.at<float>(0,0),mappedPointH.at<float>(1,0));
}





