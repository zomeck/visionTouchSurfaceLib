#ifndef DOUBLESWIPEMANAGER_H
#define DOUBLESWIPEMANAGER_H
#include "gesturemanager.h"

class DoubleSwipeManager : public GestureManager
{

    int minDist;

    Point2i prevCentroidHand, prevCentroidHand2;
    Point2i centroidHand,centroidHand2;
public:

    DoubleSwipeManager();
    //void updateStatus(FrameData &params );

    virtual bool isGestureDetected( FrameData & params );

    virtual void generateGestureData( FrameData & params );

    virtual void onStatusInit( FrameData & params );

    virtual void onStatusActive( FrameData & params );

    virtual void onStatusFinish( FrameData & params );

    virtual void onStatusInactive( FrameData & params );
};

#endif // DOUBLESWIPEMANAGER_H
