#ifndef TAPPINGMANAGER_H
#define TAPPINGMANAGER_H

#include "gesturemanager.h"
#include "kalmanfilteringswipe.h"
class TappingManager : public GestureManager
{
    KalmanFilteringSwipe kmMouse;

    Point2i cMapped,prevCMapped;
    float minXOffset, minYOffset;
public:
    TappingManager();

    //void updateStatus(FrameData &params );

    virtual bool isGestureDetected( FrameData & params );

    virtual void generateGestureData( FrameData & params );

    virtual void onStatusInit( FrameData & params );

    virtual void onStatusActive( FrameData & params );

    virtual void onStatusFinish( FrameData & params );

    virtual void onStatusInactive( FrameData & params );
};

#endif // TAPPINGMANAGER_H
