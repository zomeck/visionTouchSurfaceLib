#ifndef IOEVENTMANAGER_H
#define IOEVENTMANAGER_H

#include "eventdata.h"

class IOEventManager
{
protected:
    bool hasBeenInit;
public:
    IOEventManager();

    virtual void set( const IOEventManager & param ) = 0;

    virtual void initData()=0;

    virtual void gestureInitialize( EventData & param ) = 0;

    virtual void gestureInProgressUpdate( EventData & param )= 0;

    virtual void gestureFinalize( EventData & param ) = 0;
};

#endif // IOEVENTMANAGER_H
