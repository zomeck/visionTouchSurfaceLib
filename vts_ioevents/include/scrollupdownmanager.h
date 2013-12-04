#ifndef SCROLLUPDOWNMANAGER_H
#define SCROLLUPDOWNMANAGER_H

#include "ioeventmanager.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>


class ScrollUpDownManager : public IOEventManager
{

protected:
    Display *dpy;
public:
    ScrollUpDownManager();
    ~ScrollUpDownManager();

    void set(const IOEventManager &param );

    void initData();

    void gestureInitialize( EventData & param );

    void gestureInProgressUpdate( EventData & param );

    void gestureFinalize( EventData & param );
};

#endif // SCROLLUPDOWNMANAGER_H
