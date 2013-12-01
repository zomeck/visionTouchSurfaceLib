#include "../include/kalmanfilteringswipe.h"

KalmanFilteringSwipe::KalmanFilteringSwipe(float processNoiseVar, float measurementNoiseVar)
{
    KF = new KalmanFilter(4, 2, 0);
    KF->transitionMatrix = *(Mat_<float>(4, 4) << 1,0,1,0,   0,1,0,1,  0,0,1,0,  0,0,0,1);
    setIdentity(KF->measurementMatrix);
    setIdentity(KF->processNoiseCov, Scalar::all(processNoiseVar));
    setIdentity(KF->measurementNoiseCov, Scalar::all(measurementNoiseVar));

    measurement = Mat_<float>(2,1);
    measurement.setTo(Scalar(0));
}

void KalmanFilteringSwipe::initialiseGesture(Point initialObs, float initialStateVar)
{
    KF->statePost.at<float>(0) = initialObs.x;
    KF->statePost.at<float>(1) = initialObs.y;
    KF->statePost.at<float>(2) = 0;
    KF->statePost.at<float>(3) = 0;
    setIdentity(KF->errorCovPost, Scalar::all(initialStateVar));
    obsvec.clear();
    kalmanvec.clear();
}

Point KalmanFilteringSwipe::predictUpdateUsingObs(Point observation)
{
    KF->predict();

    measurement(0) = observation.x;
    measurement(1) = observation.y;

    obsvec.push_back(observation);
    // generate measurement
    //measurement += KF.measurementMatrix*state;

    Mat estimated = KF->correct(measurement);
    Point statePt(estimated.at<float>(0),estimated.at<float>(1));
    kalmanvec.push_back(statePt);
    return statePt;

}

Point KalmanFilteringSwipe::predictWithoutObs()
{
    Mat estimated = KF->predict();
    Point statePt(estimated.at<float>(0),estimated.at<float>(1));
    kalmanvec.push_back(statePt);
    return statePt;
}

KalmanFilteringSwipe::~KalmanFilteringSwipe()
{
    delete KF;
}
