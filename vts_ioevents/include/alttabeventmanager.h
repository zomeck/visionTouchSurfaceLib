#ifndef ALTTABEVENTMANAGER_H
#define ALTTABEVENTMANAGER_H
#include "ioeventmanager.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

class AltTabEventManager : public IOEventManager
{
    Display *dpy;
public:
    AltTabEventManager();
    ~AltTabEventManager();
    void set(const IOEventManager &param );

    void initData();

    void gestureInitialize( EventData & param );

    void gestureInProgressUpdate( EventData & param );

    void gestureFinalize( EventData & param );
};

#endif // ALTTABEVENTMANAGER_H
