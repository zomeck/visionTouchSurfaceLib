#include "swipemanager.h"

SwipeManager::SwipeManager() : GestureManager()
{
    isClicked=false;
    //state=STATUS_INACTIVE;
}

bool SwipeManager::isGestureDetected(FrameData &params)
{
    //bool movementDetected=params.contourHandsFiltered->size() == 1 && (params.contoursFingersHands->at(0).size()>1 || params.foundPalmHands->at(0));

    bool movementDetected=params.hands->size() ==1 && ( params.hands->at(0).fingers.size()>1 || params.hands->at(0).foundPalm );
    return movementDetected;

}

void SwipeManager::generateGestureData(FrameData &params)
{

    Rect bb=boundingRect( params.hands->at(0).contour );
    //this should be converted into screen coordinate system
    //centroidHand.x=bb.tl().x+bb.width/2 +150;
    //centroidHand.y=bb.tl().y+bb.height/2 + 140;
    centroidHand.x=bb.tl().x+bb.width/2;
    centroidHand.y=bb.tl().y+bb.height;//top part of the bb
    params.kinect2screen->mapPoints( centroidHand, centroidHand );

}

void SwipeManager::onStatusInit(FrameData &params)
{

    kmSwipe.initialiseGesture(centroidHand,1);

    EventData eventParam;
    eventParam.frameData=&params;
    eventParam.mousePosition=centroidHand;
    eventParam.offset=0;

    iomanagerMutex.lock();

    iomanager->gestureInitialize( eventParam );

    iomanagerMutex.unlock();

    isClicked=true;
    prevCentroidHand=centroidHand;

}

void SwipeManager::onStatusActive(FrameData &params)
{
    Point2i kmCentroidHand;
    if( gestureDetected ){
        kmCentroidHand=kmSwipe.predictUpdateUsingObs( centroidHand );
    }else{
        kmCentroidHand=kmSwipe.predictWithoutObs();
    }
    if( !isClicked ){
        //this should never happen, we initialize....
        cout<<"WARNING  isClicked should be true....."<<endl;
        isClicked=true;

        EventData eventParam;
        eventParam.frameData=&params;
        eventParam.mousePosition=kmCentroidHand;
        eventParam.offset=0;

        iomanagerMutex.lock();

        iomanager->gestureInitialize( eventParam );

        iomanagerMutex.unlock();
    }



    /*
    sampleMouseMove(kmCentroidHand.x,kmCentroidHand.y, dpy );
    */

    EventData eventParam;
    eventParam.frameData=&params;
    eventParam.mousePosition=kmCentroidHand;
    float offset=norm( kmCentroidHand-prevCentroidHand );

    if( (offset)>12 ){
        eventParam.offset=copysign( offset,kmCentroidHand.x-prevCentroidHand.x );
    }
    iomanagerMutex.lock();

    iomanager->gestureInProgressUpdate( eventParam );

    iomanagerMutex.unlock();

    prevCentroidHand=kmCentroidHand;
}

void SwipeManager::onStatusFinish(FrameData &params)
{

    EventData eventParam;
    eventParam.frameData=&params;
    eventParam.mousePosition=kmSwipe.predictWithoutObs();
    eventParam.offset=0;

    iomanagerMutex.lock();

    iomanager->gestureFinalize( eventParam );

    iomanagerMutex.unlock();

    isClicked=false;
}

void SwipeManager::onStatusInactive(FrameData &params)
{

}


/*
void SwipeManager::updateStatus(FrameData &params)
{

    bool movementDetected=params.contourHandsFiltered->size() == 1 && (params.contoursFingersHands->at(0).size()>1 || params.foundPalmHands->at(0));


    if( movementDetected ){

        resetEndCounter();
        decreaseInitCounter();

    }else{
        resetInitCounter();

        decreaseEndCounter();
    }




    Point2i centroidHand;

    if( movementDetected ){
        Rect bb=boundingRect( params.contourHandsFiltered->at(0) );
        //this should be converted into screen coordinate system
        //centroidHand.x=bb.tl().x+bb.width/2 +150;
        //centroidHand.y=bb.tl().y+bb.height/2 + 140;
        centroidHand.x=bb.tl().x+bb.width/2;
        centroidHand.y=bb.tl().y+bb.height;//top part of the bb
        params.kinect2screen->mapPoints( centroidHand, centroidHand );
    }


    if( state==STATUS_INIT ){

        kmSwipe.initialiseGesture(centroidHand,1);

        EventData eventParam;
        eventParam.frameData=&params;
        eventParam.mousePosition=centroidHand;
        eventParam.offset=0;

        iomanager->gestureInitialize( eventParam );

        isClicked=true;
        prevCentroidHand=centroidHand;

        state=STATUS_ACTIVE;

    }else if( state==STATUS_ACTIVE ){

        Point2i kmCentroidHand;
        if( movementDetected ){
            kmCentroidHand=kmSwipe.predictUpdateUsingObs( centroidHand );
        }else{
            kmCentroidHand=kmSwipe.predictWithoutObs();
        }
        if( !isClicked ){
            //this should never happen, we initialize....
            cout<<"WARNING  isClicked should be true....."<<endl;
            isClicked=true;

            EventData eventParam;
            eventParam.frameData=&params;
            eventParam.mousePosition=kmCentroidHand;
            eventParam.offset=0;

            iomanager->gestureInitialize( eventParam );
        }



        /*
        sampleMouseMove(kmCentroidHand.x,kmCentroidHand.y, dpy );


EventData eventParam;
eventParam.frameData=&params;
eventParam.mousePosition=kmCentroidHand;
eventParam.offset=norm( kmCentroidHand-prevCentroidHand );

iomanager->gestureInProgressUpdate( eventParam );

prevCentroidHand=kmCentroidHand;

}else if( state==STATUS_FINISH ){


    EventData eventParam;
    eventParam.frameData=&params;
    eventParam.mousePosition=kmSwipe.predictWithoutObs();
    eventParam.offset=0;

    iomanager->gestureFinalize( eventParam );
    isClicked=false;

    state=STATUS_INACTIVE;
}

/*
    int initialCallState=state;

    //temporal
    //bool isDrag=true;

    //detected 1 hand that has more than 1 fingertip
    if( params.contourHandsFiltered->size() == 1 && (params.contoursFingersHands->at(0).size()>1 || params.foundPalmHands->at(0)) ){
        state=STATUS_ACTIVE;

        Rect bb=boundingRect( params.contourHandsFiltered->at(0) );

        Point2i centroidHand;

        //this should be converted into screen coordinate system
        //centroidHand.x=bb.tl().x+bb.width/2 +150;
        //centroidHand.y=bb.tl().y+bb.height/2 + 140;
        centroidHand.x=bb.tl().x+bb.width/2;
        centroidHand.y=bb.tl().y+bb.height;//top part of the bb
        params.kinect2screen->mapPoints( centroidHand, centroidHand );

        if( initialCallState != state ){
            //initialize movement

            kmSwipe.initialiseGesture(centroidHand,1);


            /*
            sampleMouseMove( centroidHand.x, centroidHand.y, dpy );
            XFlush(dpy);
            XTestFakeButtonEvent (dpy, 1, true,  CurrentTime);
            XFlush(dpy);


            EventData eventParam;
            eventParam.frameData=&params;
            eventParam.mousePosition=centroidHand;
            eventParam.offset=0;

            iomanager->gestureInitialize( eventParam );

            isClicked=true;
            prevCentroidHand=centroidHand;
        }else{

            if( !isClicked ){
                //this should never happen, we initialize....
                cout<<"WARNING  isClicked should be true....."<<endl;

                /*
                XTestFakeButtonEvent (dpy, 1, true,  CurrentTime);
                XFlush(dpy);


                isClicked=true;

                EventData eventParam;
                eventParam.frameData=&params;
                eventParam.mousePosition=centroidHand;
                eventParam.offset=0;

                iomanager->gestureInitialize( eventParam );
            }

            Point2i kmCentroidHand=kmSwipe.predictUpdateUsingObs( centroidHand );

            /*
            sampleMouseMove(kmCentroidHand.x,kmCentroidHand.y, dpy );


            EventData eventParam;
            eventParam.frameData=&params;
            eventParam.mousePosition=centroidHand;
            eventParam.offset=0;

            iomanager->gestureInProgressUpdate( eventParam );

            prevCentroidHand=kmCentroidHand;



        }

    }else{

        if( isClicked ){

            /*
            XTestFakeButtonEvent (dpy, 1, false,  CurrentTime);
            XFlush(dpy);

            EventData eventParam;
            eventParam.frameData=&params;
            eventParam.mousePosition=Point2i(0,0);
            eventParam.offset=0;

            iomanager->gestureFinalize( eventParam );
            isClicked=false;

        }
        state=STATUS_INACTIVE;
    }


}
*/
