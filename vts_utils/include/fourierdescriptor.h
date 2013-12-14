#ifndef FOURIERDESCRIPTOR_H
#define FOURIERDESCRIPTOR_H

#include <fstream>
#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/**
 * @brief The FourierDescriptor class computes the fourier descriptor of a hand and matches it with a class of a dictionary
 *
 *
 The class implements several distance metrics (L2 norm,L1 norm and canberra) that can be used in order to know the distance from a query binary mask to a dictionary class.

 */
class FourierDescriptor
{
public:
    FourierDescriptor();
    FourierDescriptor(const string &fileName);
    void loadDictionary(const string &fileName);
    int findMatchInDictionary(vector<Point2i> &boundary, string &distMetric, double &distance, int numFDtoUse=0, int newBoundaryLength=64);
    void calcCentroidDistFD(vector<Point2i> &boundary, int numFD, Mat &dst, int newBoundaryLength=64);
    int findMatchInDictionary(Mat &testFD, string &distMetric, double &distance );
    ~FourierDescriptor();


private:
    int sizeDict,numFDdict;
    Mat *FDdict;
    vector<int> dictClass;

};

#endif // FOURIERDESCRIPTOR_H
