#ifndef KALMANFILTERINGSWIPE_H
#define KALMANFILTERINGSWIPE_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <vector>
#include <iostream>



using namespace cv;
using namespace std;

class KalmanFilteringSwipe
{
public:
    //KalmanFilteringSwipe();
    KalmanFilteringSwipe(float processNoiseVar =1e-4, float measurementNoiseVar =8.5e-2);
    virtual void initialiseGesture(Point initialObs, float initialStateVar);
    virtual Point predictUpdateUsingObs(Point observation);
    virtual Point predictWithoutObs();
    ~KalmanFilteringSwipe();

//private:
    KalmanFilter *KF;
    Mat_<float> measurement;
    vector<Point> obsvec,kalmanvec;

};

#endif // KALMANFILTERINGSWIPE_H
