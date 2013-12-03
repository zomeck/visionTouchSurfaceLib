#ifndef FOURIERDESCRIPTOR_H
#define FOURIERDESCRIPTOR_H

#include <fstream>
#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


class FourierDescriptor
{
public:
    FourierDescriptor();
    FourierDescriptor(const string &fileName);
    void loadDictionary(const string &fileName);
    int findMatchInDictionary(vector<Point2i> &boundary, string &distMetric,Mat *result=0, int newBoundaryLength=64);
    void calcCentroidDistFD(vector<Point2i> &boundary, int numFD, Mat &dst, int newBoundaryLength=64);
    int findMatchInDictionary(Mat &testFD, string &distMetric, Mat *result=0);
    ~FourierDescriptor();

private:
    int sizeDict,numFDdict;
    Mat *FDdict;
    vector<int> dictClass;

};

#endif // FOURIERDESCRIPTOR_H
