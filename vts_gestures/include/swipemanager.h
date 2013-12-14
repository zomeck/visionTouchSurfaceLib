#ifndef SWIPEMANAGER_H
#define SWIPEMANAGER_H

#include "gesturemanager.h"

#include "kalmanfilteringswipe.h"

/**
 * @brief The SwipeManager class inherits from GestureManager to implement a hand swiping gesture
 *
 When the gesture is detected it provides the IOEventManager the top center position of the hand as mouse position and the distance between the hand position in the
 current frame and the hand posistion from the previous frame
 */
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


    /**
     * @brief isGestureDetected determines whether the gesutre has been detected or not
     * @param params data of the frame needed for the gesture
     * @return true if only one hand is detected with at least more than one fingertip or a palm
     */
    virtual bool isGestureDetected( FrameData & params );

    virtual void generateGestureData( FrameData & params );

    virtual void onStatusInit( FrameData & params );

    virtual void onStatusActive( FrameData & params );

    virtual void onStatusFinish( FrameData & params );

    virtual void onStatusInactive( FrameData & params );
};

#endif // SWIPEMANAGER_H
