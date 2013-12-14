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
    line( params.rgbDebug, Point2i( 1, 6 ), Point2i( params.rgbDebug.cols-1, 6 ),Scalar(255,255,255),4 );

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
    line( params.rgbDebug, Point2i( 1, 6 ), Point2i( params.rgbDebug.cols-1, 6 ),Scalar(255,255,255),4 );

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

    line( params.rgbDebug, Point2i( 1, 6 ), Point2i( params.rgbDebug.cols-1, 6 ),Scalar(255,255,255),4 );

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

