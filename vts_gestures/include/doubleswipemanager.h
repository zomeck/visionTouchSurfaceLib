#ifndef DOUBLESWIPEMANAGER_H
#define DOUBLESWIPEMANAGER_H
#include "gesturemanager.h"

/**
 * @brief The DoubleSwipeManager class inherits from GestureManager to implement a double handed gesture
 *
    When the gesture is detected it provides the centroid of both hands as the mouse position and the offset variable is set with the diference of distances
    between the hands at the current frame with the hands from the previous frame.
 */
class DoubleSwipeManager : public GestureManager
{

    int minDist;

    Point2i prevCentroidHand, prevCentroidHand2;
    Point2i centroidHand,centroidHand2;
public:

    DoubleSwipeManager();
    //void updateStatus(FrameData &params );

    /**
     * @brief isGestureDetected determines whether the gesutre has been detected or not
     * @param params  data of the frame needed for the gesture
     * @return true if and only if two hands are detected and each of them has at least a finger tip or a palm detected
     */
    virtual bool isGestureDetected( FrameData & params );

    virtual void generateGestureData( FrameData & params );

    virtual void onStatusInit( FrameData & params );

    virtual void onStatusActive( FrameData & params );

    virtual void onStatusFinish( FrameData & params );

    virtual void onStatusInactive( FrameData & params );
};

#endif // DOUBLESWIPEMANAGER_H
