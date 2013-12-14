#ifndef MONITORHANDSMANAGER_H
#define MONITORHANDSMANAGER_H

#include "gesturemanager.h"

/**
 * @brief The MonitorHandsManager class inherits from GestureManager to implement a hand monitoring gesture
 *
 This object does not give any mouse position it is a class that will trigger the IOEventManager when a hand is detected. This gesture can be used mainly for
 debugging the system or determine the false positives
 */
class MonitorHandsManager: public GestureManager
{
public:
    MonitorHandsManager();

    /**
     * @brief isGestureDetected determines whether the gesutre has been detected or not
     * @param params data of the frame needed for the gesture
     * @return true if one hand or more are detected
     */
    virtual bool isGestureDetected( FrameData & params );

    virtual void generateGestureData( FrameData & params );

    virtual void onStatusInit( FrameData & params );

    virtual void onStatusActive( FrameData & params );

    virtual void onStatusFinish( FrameData & params );

    virtual void onStatusInactive( FrameData & params );
};

#endif // MONITORHANDSMANAGER_H
