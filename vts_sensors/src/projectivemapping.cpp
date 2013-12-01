#include "projectivemapping.h"

ProjectiveMapping::ProjectiveMapping()
{
    rect_corners.assign( 4,Point2f(0,0) );
    transformationMatrix=Mat::zeros(1,1,CV_32F);
}

ProjectiveMapping::ProjectiveMapping(int width, int height)
{
    rect_corners.reserve(4);
    rect_corners.push_back(cvPoint(width,height));  //bottom right
    rect_corners.push_back(cvPoint(0,height));      //bottom left
    rect_corners.push_back(cvPoint(0,0));           //top left
    rect_corners.push_back(cvPoint(width,0));       //top right

}

ProjectiveMapping::ProjectiveMapping(const ProjectiveMapping &param)
{

    transformationMatrix=param.transformationMatrix.clone();
    rect_corners=param.rect_corners;

}

void ProjectiveMapping::calculateMapping(const vector<Point2f> &quadrangle_corners)
{
    if(rect_corners.at(2).x==0 && rect_corners.at(2).y==0)
    {
        cerr<<"Warning: In ProjectiveMapping::calculateMapping, width and height are uninitialised/both zero."<<endl;
    }

    // enforce quadrangle corners to be ordered as (top left, top right, bottom right, bottom left)
    //left corners have the two lowest x coordinates
    //right corners have the two highest x coordinates
    //top corners have the two lowest y coordinates
    //bottom corners have the two highest y coordinates

    if(quadrangle_corners.size()!=4)
        cerr<<"Warning: In ProjectiveMapping::calculateMapping, four corners of the quadrangle are needed."<<endl;

    float minX=1e9;
    float minY=1e9;
    int minXidx=-1;
    int minYidx=-1;
    //find mins
    for(int i=0;i<4;i++)
    {
        if( quadrangle_corners[i].x<minX){
            minX=quadrangle_corners[i].x;
            minXidx=i;
        }
        if( quadrangle_corners[i].y<minY){
            minY=quadrangle_corners[i].y;
            minYidx=i;
        }
    }
    float minX2=1e9;
    float minY2=1e9;
    int minX2idx=-1;
    int minY2idx=-1;
    //find second-mins
    for(int i=0;i<4;i++)
    {
        if( quadrangle_corners[i].x<minX2 && i!=minXidx){
            minX2=quadrangle_corners[i].x;
            minX2idx=i;
        }
        if( quadrangle_corners[i].y<minY2 && i!=minYidx){
            minY2=quadrangle_corners[i].y;
            minY2idx=i;
        }
    }

    //determine topleft
    Point2f topleft, topright, bottomleft, bottomright;
    for(int i=0;i<4;i++){
        bool left= (i==minXidx || i==minX2idx);
        bool top= (i==minYidx || i==minY2idx);

        if(left){
            if(top){
                topleft=quadrangle_corners[i];
            }
            else
                bottomleft=quadrangle_corners[i];

        }
        else{
            if(top){
                topright=quadrangle_corners[i];
            }
            else
                bottomright=quadrangle_corners[i];
        }
    }
    vector<Point2f> quadrangle_corners_organised;
    quadrangle_corners_organised.push_back(topleft);
    quadrangle_corners_organised.push_back(topright);
    quadrangle_corners_organised.push_back(bottomright);
    quadrangle_corners_organised.push_back(bottomleft);

    transformationMatrix=getPerspectiveTransform(quadrangle_corners_organised, rect_corners);
}


void ProjectiveMapping::mapPoints(vector<Point2f> &points, vector<Point2f> &mappedPoints)
{
    /*
    if(transformationMatrix. Mat::zeros(1,1,CV_32F))
    {
        cerr<<"Error: In ProjectiveMapping::mapPoints, calculateMapping must be called first to initialise the transformation matrix."<<endl;
    }
    else
    {
        perspectiveTransform(points, mappedPoints, transformationMatrix);
    }
    */
    perspectiveTransform(points, mappedPoints, transformationMatrix);
}

void ProjectiveMapping::mapPoints(Point2i &point, Point2i &mappedPoint)
{
    /*
    if(transformationMatrix==Mat::zeros(1,1,CV_32F))
    {
        cerr<<"Error: In ProjectiveMapping::mapPoints, calculateMapping must be called first to initialise the transformation matrix."<<endl;
    }
    else{
        vector<Point2f> pointVec, mappedPointVec;
        pointVec.push_back(point);
        mappedPointVec.push_back(mappedPoint);
        perspectiveTransform(pointVec, mappedPointVec, transformationMatrix);
        mappedPoint=mappedPointVec.at(0);
    }
    */
    vector<Point2f> pointVec, mappedPointVec;
    Point2f tmp;
    tmp.x=point.x;
    tmp.y=point.y;
    pointVec.push_back(tmp);
    mappedPointVec.push_back(mappedPoint);
    perspectiveTransform(pointVec, mappedPointVec, transformationMatrix);
    mappedPoint.x=mappedPointVec.at(0).x;
    mappedPoint.y=mappedPointVec.at(0).y;
}

void ProjectiveMapping::warpMatrix(Mat &matrix, Mat &warpedMatrix, Size &warpedSize)
{
    /*
    if(transformationMatrix==Mat::zeros(1,1,CV_32F))
    {
        cerr<<"Error: In ProjectiveMapping::warpMatrix, calculateMapping must be called first to initialise the transformation matrix."<<endl;
    }
    */
    warpPerspective(matrix, warpedMatrix, transformationMatrix, warpedSize, INTER_LINEAR, BORDER_CONSTANT);
}

