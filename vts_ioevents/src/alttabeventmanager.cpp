#include "alttabeventmanager.h"

AltTabEventManager::AltTabEventManager():IOEventManager()
{
    dpy=NULL;
}

AltTabEventManager::~AltTabEventManager()
{
    if( dpy!=NULL ){
        XCloseDisplay (dpy);
    }
    dpy=NULL;
}

/*
void AltTabEventManager::set(const IOEventManager &param)
{

}
*/

void AltTabEventManager::initData()
{
    if( dpy==NULL ){
        dpy = XOpenDisplay (NULL);
    }
    hasBeenInit=true;

}

void AltTabEventManager::gestureInitialize(EventData &param)
{
    if(!hasBeenInit){
        initData();
        hasBeenInit=true;
    }

    int kc = XKeysymToKeycode (dpy, XK_Alt_L );
    XTestFakeKeyEvent(dpy, kc, True, CurrentTime);
    XFlush(dpy);

}

void AltTabEventManager::gestureInProgressUpdate(EventData &param)
{

    if( param.offset>0 ){

        int kc = XKeysymToKeycode (dpy, XK_Tab );
        XTestFakeKeyEvent(dpy, kc, True, CurrentTime);
        XTestFakeKeyEvent(dpy, kc, False, CurrentTime);
        XFlush(dpy);


    }else if( param.offset<0 ){
        int kcTab = XKeysymToKeycode (dpy, XK_Tab );
        int kcShift= XKeysymToKeycode (dpy, XK_Shift_L );
        XTestFakeKeyEvent(dpy, kcShift, True, CurrentTime);
        XTestFakeKeyEvent(dpy, kcTab, True, CurrentTime);
        XTestFakeKeyEvent(dpy, kcTab, False, CurrentTime);
        XTestFakeKeyEvent(dpy, kcShift, False, CurrentTime);
        XFlush(dpy);

    }
}

void AltTabEventManager::gestureFinalize(EventData &param)
{

    int kc = XKeysymToKeycode (dpy, XK_Alt_L );
    XTestFakeKeyEvent(dpy, kc, False, CurrentTime);
    XFlush(dpy);
}
