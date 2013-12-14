#include "singleclickholdeventmanager.h"

SingleClickHoldEventManager::SingleClickHoldEventManager():IOEventManager()
{
    dpy=NULL;
    button=1;
}

SingleClickHoldEventManager::~SingleClickHoldEventManager()
{
    if( dpy!=NULL ){
        XCloseDisplay (dpy);
    }
    dpy=NULL;
}

/*
void SingleClickHoldEventManager::set(const IOEventManager &param)
{

}
*/
void SingleClickHoldEventManager::setButton(int pbutton)
{
    if( pbutton>0 && pbutton<7 ){
        button=pbutton;
    }else{
        //print some err
    }

}

void SingleClickHoldEventManager::initData()
{
    if( dpy==NULL ){
        dpy = XOpenDisplay (NULL);
    }
    hasBeenInit=true;
}

void SingleClickHoldEventManager::gestureInitialize(EventData &param)
{

    if(!hasBeenInit){
        initData();
        hasBeenInit=true;
    }

    XTestFakeMotionEvent(dpy, 0, param.mousePosition.x, param.mousePosition.y, CurrentTime);
    XFlush(dpy);
    XTestFakeButtonEvent (dpy, button, true,  CurrentTime);
    XFlush(dpy);
}

void SingleClickHoldEventManager::gestureInProgressUpdate(EventData &param)
{

    XTestFakeMotionEvent(dpy, 0, param.mousePosition.x, param.mousePosition.y, CurrentTime);
    XFlush(dpy);
}

void SingleClickHoldEventManager::gestureFinalize(EventData &param)
{

    XTestFakeButtonEvent (dpy, button, false,  CurrentTime);
    XFlush(dpy);
}
