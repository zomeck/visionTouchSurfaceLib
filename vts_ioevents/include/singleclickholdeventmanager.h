#ifndef INCLUDESINGLECLICKHOLDEVENTMANAGER_H
#define INCLUDESINGLECLICKHOLDEVENTMANAGER_H
#include "ioeventmanager.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

/**
 * @brief The SingleClickHoldEventManager class inherits from IOEventManager to implement a button click and hold using Xlib
 *
 The class will simulate a mouse click of the button set as parameter, the default button is the left button (button 1 in Xlib).
 The class will start clicking when calling gestureInitialize and stop the clicking when gestureFinalize. Call gestureInProgressUpdate will update
 the mouse position.
 */
class SingleClickHoldEventManager : public IOEventManager
{
protected:
    Display *dpy;
    int button;
public:
    SingleClickHoldEventManager();
    ~SingleClickHoldEventManager();

    //void set(const IOEventManager &param );

    void setButton( int pbutton );

    void initData();

    void gestureInitialize( EventData & param );

    void gestureInProgressUpdate( EventData & param );

    void gestureFinalize( EventData & param );
};

#endif // INCLUDESINGLECLICKHOLDEVENTMANAGER_H
