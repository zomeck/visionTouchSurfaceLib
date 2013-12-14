#ifndef TAPPINGMANAGER_H
#define TAPPINGMANAGER_H

#include "gesturemanager.h"
#include "kalmanfilteringswipe.h"

/**
 * @brief The TappingManager class inherits from GestureManager to implement a tapping gesture
 *
 When the gesture is detected it provides the IOEventManager the fingertip as mouse position and the distance between the fingertip in the
 current frame and the fingertip from the previous frame
 */
class TappingManager : public GestureManager
{
    KalmanFilteringSwipe kmMouse;

    Point2i cMapped,prevCMapped;
    float minXOffset, minYOffset;
public:
    TappingManager();

    //void updateStatus(FrameData &params );

    /**
     * @brief isGestureDetected determines whether the gesutre has been detected or not
     * @param params data of the frame needed for the gesture
     * @return true if only one hand is detected with only one fingertip and no palm is found
     */
    virtual bool isGestureDetected( FrameData & params );

    virtual void generateGestureData( FrameData & params );

    virtual void onStatusInit( FrameData & params );

    virtual void onStatusActive( FrameData & params );

    virtual void onStatusFinish( FrameData & params );

    virtual void onStatusInactive( FrameData & params );
};

#endif // TAPPINGMANAGER_H
