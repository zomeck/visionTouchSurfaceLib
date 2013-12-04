#ifndef MONITORHANDSMANAGER_H
#define MONITORHANDSMANAGER_H

#include "gesturemanager.h"
class MonitorHandsManager: public GestureManager
{
public:
    MonitorHandsManager();

    virtual bool isGestureDetected( FrameData & params );

    virtual void generateGestureData( FrameData & params );

    virtual void onStatusInit( FrameData & params );

    virtual void onStatusActive( FrameData & params );

    virtual void onStatusFinish( FrameData & params );

    virtual void onStatusInactive( FrameData & params );
};

#endif // MONITORHANDSMANAGER_H
