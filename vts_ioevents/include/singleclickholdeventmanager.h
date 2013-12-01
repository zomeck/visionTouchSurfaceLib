#ifndef INCLUDESINGLECLICKHOLDEVENTMANAGER_H
#define INCLUDESINGLECLICKHOLDEVENTMANAGER_H
#include "ioeventmanager.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

class SingleClickHoldEventManager : public IOEventManager
{
protected:
    Display *dpy;
    int button;
public:
    SingleClickHoldEventManager();
    ~SingleClickHoldEventManager();

    void set(const IOEventManager &param );

    void setButton( int pbutton );

    void initData();

    void gestureInitialize( EventData & param );

    void gestureInProgressUpdate( EventData & param );

    void gestureFinalize( EventData & param );
};

#endif // INCLUDESINGLECLICKHOLDEVENTMANAGER_H
