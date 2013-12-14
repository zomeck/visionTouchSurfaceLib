#include "scrollupdownmanager.h"

ScrollUpDownManager::ScrollUpDownManager() :IOEventManager()
{
    dpy=NULL;
}

ScrollUpDownManager::~ScrollUpDownManager()
{
    if( dpy!=NULL ){
        XCloseDisplay (dpy);
    }
    dpy=NULL;
}

/*
void ScrollUpDownManager::set(const IOEventManager &param)
{
    /*
    if( dpy!=NULL ){
        XCloseDisplay (dpy);
    }
    dpy=param.dpy.;


}
*/

void ScrollUpDownManager::initData()
{
    if( dpy==NULL ){
        dpy = XOpenDisplay (NULL);
    }
    hasBeenInit=true;

}

void ScrollUpDownManager::gestureInitialize(EventData &param)
{

    if(!hasBeenInit){
        initData();
        hasBeenInit=true;
    }
}

void ScrollUpDownManager::gestureInProgressUpdate(EventData &param)
{

    int button;
    if(  param.offset>0 ){
        button=4;
    }else if( param.offset<0 ){
        button=5;
    }else{
        button=-1;
    }

    //sampleMouseMove( cursorCoordsKinect.x,  cursorCoordsKinect.y );

    if( button!=-1 ){
        XTestFakeMotionEvent(dpy, 0, param.mousePosition.x, param.mousePosition.y, CurrentTime);
        XFlush(dpy);
        //sampleMouseMove( centroidHand.x,  centroidHand.y );
        //cout<<cursorCoordsKinect<<endl;

        XTestFakeButtonEvent (dpy, button, True,  CurrentTime);
        XFlush(dpy);
        XTestFakeButtonEvent (dpy, button, False, CurrentTime);
        XFlush(dpy);
    }

}

void ScrollUpDownManager::gestureFinalize(EventData &param)
{


}
