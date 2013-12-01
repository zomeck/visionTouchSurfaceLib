#ifndef SWIPEMANAGER_H
#define SWIPEMANAGER_H

#include "gesturemanager.h"

#include "kalmanfilteringswipe.h"

class SwipeManager : public GestureManager
{

protected:
    bool isClicked;
    Point2i prevCentroidHand;
    KalmanFilteringSwipe kmSwipe;
    Point2i centroidHand;
public:
    SwipeManager();

    //void updateStatus(FrameData &params );


    virtual bool isGestureDetected( FrameData & params );

    virtual void generateGestureData( FrameData & params );

    virtual void onStatusInit( FrameData & params );

    virtual void onStatusActive( FrameData & params );

    virtual void onStatusFinish( FrameData & params );

    virtual void onStatusInactive( FrameData & params );
};

#endif // SWIPEMANAGER_H
