#include "fourierdescriptor.h"

FourierDescriptor::FourierDescriptor()
{
    FDdict= NULL;
}

FourierDescriptor::FourierDescriptor(const string &fileName)
{
    FDdict= NULL;
    loadDictionary(fileName);
}

void FourierDescriptor::loadDictionary(const string &fileName)
{
    ifstream ifs(fileName.c_str(),ifstream::in);
    if(!(ifs>>sizeDict)){
        cerr<<"Couldn't read Fourier descriptor dictionary size."<<endl;
    }

    if(!(ifs>>numFDdict)){
        cerr<<"Couldn't read Fourier descriptor length from dictionary."<<endl;
    }

    delete FDdict;
    FDdict=new Mat(sizeDict, numFDdict, CV_32F);

    int classTemp;
    for(int i=0; i<sizeDict; i++){
        if(!(ifs>>classTemp)){
            cerr<<"Couldn't read from dictionary."<<endl;
        }else{
            dictClass.push_back(classTemp);
        }

        for(int j=0; j<numFDdict; j++){
            if(!(ifs>>FDdict->at<float>(i,j))){
                cerr<<"Couldn't read from dictionary."<<endl;
            }
        }
    }
}

void FourierDescriptor::calcCentroidDistFD(vector<Point2i> &boundary, int numFD, Mat &dst, int newBoundaryLength)
{
    //downsample boundary and get centroid of points
    int L=boundary.size();
    if(newBoundaryLength>L){
        cerr<<"New boundary length exceeds existing length. Only downsampling is implemented."<<endl;
    }

    float stepsize=L/(float)newBoundaryLength;
    vector<Point2i> sampledList;
    Point2d centroid(0,0);
    for(float i=0; i<L; i+=stepsize){
        int inti=round(i);
        Point2d currPt=boundary[inti];
        sampledList.push_back(currPt);
        centroid=centroid+currPt;
    }
    centroid=centroid*(1/(double)newBoundaryLength);

    //get distances to centroid
    Mat distToCentroid(1,newBoundaryLength,CV_32F);
    for(int i=0; i<newBoundaryLength; i++){
        distToCentroid.at<float>(i)=sqrt(sampledList[i].x*sampledList[i].x+sampledList[i].y*sampledList[i].y);
    }

    //perform FT
    Mat FDs_all;
    dft(distToCentroid,FDs_all,DFT_SCALE);

    //get fourier descriptors (first numFD non-DC ones)
    Mat FDs(1, numFD, CV_32F);
    for(int i=0; i<numFD; i++){
        float real=FDs_all.at<float>(2*i+1);
        float comp=FDs_all.at<float>(2*i+2);
        FDs.at<float>(i)=sqrt(real*real+comp*comp)/FDs_all.at<float>(0);
    }
    dst= FDs.clone();
}

int FourierDescriptor::findMatchInDictionary(vector<Point2i> &boundary, string &distMetric, Mat *result, int newBoundaryLength)
{
    //get CDFD
    Mat testFD;
    calcCentroidDistFD(boundary,numFDdict,testFD,newBoundaryLength);
    int matchClass=findMatchInDictionary(testFD,distMetric,result);
    return matchClass;

}

int FourierDescriptor::findMatchInDictionary(Mat &testFD, string &distMetric, Mat *result)
{
    int numFD=0;
    if(testFD.rows==1)
    {
        numFD=testFD.cols;
    }else{
        cerr<<"Fourier descriptor to test must be Mat with one row."<<endl;
    }

    if(numFDdict!=numFD){
        cerr<<"Fourier descriptor dictionary uses a different length."<<endl;
    }

    //compare with dictionary
    Mat FDtoTest(sizeDict, numFDdict, CV_32F);
    repeat(testFD, sizeDict, 1, FDtoTest);
    Mat dist;
    if(distMetric.compare("2norm")==0)
    {
        Mat diff=FDtoTest-(*FDdict);
        Mat diffSq, sumDiffSq;
        cv::pow(diff, 2, diffSq);
        reduce(diffSq,sumDiffSq,1, CV_REDUCE_SUM);
        cv::pow(sumDiffSq, 0.5, dist);

    }
    else{
        if(distMetric.compare("1norm")==0)
        {
            Mat diff=FDtoTest-(*FDdict);
            Mat diffAbs;
            diffAbs = abs(diff);
            reduce(diffAbs,dist,1, CV_REDUCE_SUM);
        }
        else{
            if(distMetric.compare("canberra")==0)
            {
                Mat diff=FDtoTest-(*FDdict);
                Mat diffAbs, diffAbsScale;
                diffAbs = abs(diff);
                Mat scale = 1/(abs(FDtoTest)+abs((*FDdict)));
                multiply(diffAbs,scale,diffAbsScale);
                reduce(diffAbsScale,dist,1, CV_REDUCE_SUM);
            }
            else{
                cerr<<"Distance metric not recognised."<<endl;
                return -1;
            }
        }
    }


    double minVal, maxVal;
    int minIdx, maxIdx;
    minMaxIdx(dist,&minVal, &maxVal, &minIdx, &maxIdx);
    int matchClass = dictClass[minIdx];
    *result = dist;
    return matchClass;

}

FourierDescriptor::~FourierDescriptor()
{
 delete FDdict;
}








