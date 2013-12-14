#ifndef SCROLLUPDOWNMANAGER_H
#define SCROLLUPDOWNMANAGER_H

#include "ioeventmanager.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

/**
 * @brief The ScrollUpDownManager class inherits from IOEventManager to implement a scrolling up and down event using Xlib
 *
 This class will simulate scrolling up(wheel up - button 4 of Xlib) when offset values are positive and scrolling down(wheel down -button 5 of Xlib) when offset
values are negative. When offset is 0 no action is taken.

 */
class ScrollUpDownManager : public IOEventManager
{

protected:
    Display *dpy;
public:
    ScrollUpDownManager();
    ~ScrollUpDownManager();

    //void set(const IOEventManager &param );

    void initData();

    void gestureInitialize( EventData & param );

    void gestureInProgressUpdate( EventData & param );

    void gestureFinalize( EventData & param );
};

#endif // SCROLLUPDOWNMANAGER_H
