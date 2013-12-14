#ifndef ALTTABEVENTMANAGER_H
#define ALTTABEVENTMANAGER_H
#include "ioeventmanager.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

/**
 * @brief The AltTabEventManager class inherits from IOEventManager to implement Alt+tab key shortcut using Xlib
 *
 This class will simulate the alt+tab or alt+shift+tab depending if the offset given when calling AltTabEventManager::gestureInProgressUpdate() is positive or
 negative. Tab pressing is going to be simulated as many tames as AltTabEventManager::gestureInProgressUpdate() is called
 */
class AltTabEventManager : public IOEventManager
{
    Display *dpy;
public:
    AltTabEventManager();
    ~AltTabEventManager();

    //void set(const IOEventManager &param );

    void initData();

    void gestureInitialize( EventData & param );

    void gestureInProgressUpdate( EventData & param );

    void gestureFinalize( EventData & param );
};

#endif // ALTTABEVENTMANAGER_H
