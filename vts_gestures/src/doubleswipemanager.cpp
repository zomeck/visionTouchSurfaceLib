#include "doubleswipemanager.h"

DoubleSwipeManager::DoubleSwipeManager(): GestureManager()
{

    minDist=20;
    //state=STATUS_INACTIVE;
}

bool DoubleSwipeManager::isGestureDetected(FrameData &params)
{
    /*
    bool movementDetected= params.contourHandsFiltered->size() == 2 &&
            (params.contoursFingersHands->at(0).size()>0 || params.foundPalmHands->at(0) )&&
            (params.contoursFingersHands->at(1).size()>0 || params.foundPalmHands->at(1) );//probably should add that both have fingertips...
*/
   // bool movementDetected=params.contourHandsFiltered->size() == 1 && (params.contoursFingersHands->at(0).size()>=1 || params.foundPalmHands->at(0)) && (params.contoursFingersHands->at(1).size()>=1 || params.foundPalmHands->at(1));

    //bool movementDetected= params.contourHandsFiltered->size() == 2 && (params.contoursFingersHands->at(0).size()>=1 || params.foundPalmHands->at(0)) && (params.contoursFingersHands->at(1).size()>=1 || params.foundPalmHands->at(1));
    bool movementDetected=params.hands->size() ==2 &&
            ( params.hands->at(0).fingers.size()>=1 || params.hands->at(0).foundPalm )&&
            ( params.hands->at(1).fingers.size()>=1 || params.hands->at(1).foundPalm );
    return movementDetected;

}

void DoubleSwipeManager::generateGestureData(FrameData &params)
{



    if( gestureDetected ){

        /*
        Rect bb=boundingRect( params.hands->at(0).contour );
        Rect bb2=boundingRect( params.hands->at(1).contour  );

        centroidHand.x=bb.x+bb.width/2;
        centroidHand.y=bb.y+bb.height/2;
        */
        centroidHand.x=params.hands->at(0).centroid.x;
        centroidHand.y=params.hands->at(0).centroid.y;
        params.kinect2screen->mapPoints( centroidHand, centroidHand );

        /*
        centroidHand2.x=bb2.x+bb2.width/2;
        centroidHand2.y=bb2.y+bb2.height/2;
        */
        centroidHand2.x=params.hands->at(1).centroid.x;
        centroidHand2.y=params.hands->at(1).centroid.y;
        params.kinect2screen->mapPoints( centroidHand2, centroidHand2 );

    }
}

void DoubleSwipeManager::onStatusInit(FrameData &params)
{

    line( params.rgbDebug, Point2i( 1, 10 ), Point2i( params.rgbDebug.cols-1, 10 ),Scalar(0,255,0),4 );

    prevCentroidHand=centroidHand;
    prevCentroidHand2=centroidHand2;
    Point2i cursorCoordsKinect= (centroidHand2 + centroidHand);
    cursorCoordsKinect.x/=2;
    cursorCoordsKinect.y/=2;
    //kmHands.initialiseGesture( center, minDist);

    //TODO: set data into eventData
    EventData eventData;
    eventData.frameData=&params;
    eventData.mousePosition=cursorCoordsKinect;
    eventData.offset=0;

    iomanagerMutex.lock();

    iomanager->gestureInitialize( eventData );

    iomanagerMutex.unlock();

}

void DoubleSwipeManager::onStatusActive(FrameData &params)
{
    line( params.rgbDebug, Point2i( 1, 10 ), Point2i( params.rgbDebug.cols-1, 10 ),Scalar(0,255,0),4 );

    if( !gestureDetected ){
        centroidHand=prevCentroidHand;
        centroidHand2=prevCentroidHand2;
    }

    Point2i distCentroids=centroidHand-prevCentroidHand;
    Point2i tmpDistMixed=centroidHand-prevCentroidHand2;
    //they are mapped the other way round
    if( cv::norm( tmpDistMixed )< cv::norm( distCentroids ) ){

        Point2i tmp;
        tmp=centroidHand;
        centroidHand=centroidHand2;
        centroidHand2=tmp;
    }

    bool handHasMoved = norm( centroidHand - prevCentroidHand )>minDist;
    bool hand2HasMoved = norm( centroidHand2 - prevCentroidHand2 )>minDist;

    EventData eventData;
    eventData.frameData=&params;
    eventData.offset=0;

    if ( handHasMoved || hand2HasMoved ){
        Point2i actDistance=centroidHand-centroidHand2;
        Point2i prevDistance=prevCentroidHand-prevCentroidHand2;
        //Point2i cursorCoordsKinect=centroidHand;
        Point2i cursorCoordsKinect= (centroidHand2 + centroidHand);
        cursorCoordsKinect.x/=2;
        cursorCoordsKinect.y/=2;
        float zoomFactor=cv::norm(actDistance) - cv::norm( prevDistance );

        //JOB of the IOEventManager assigned
        if( abs( zoomFactor )> minDist*1.5 ){
            eventData.offset=zoomFactor;
        }

        eventData.mousePosition=cursorCoordsKinect;

    }else{
        Point2i cursorCoordsKinect=(prevCentroidHand2 + prevCentroidHand);
        cursorCoordsKinect.x/=2;
        cursorCoordsKinect.y/=2;
        eventData.mousePosition=cursorCoordsKinect;
    }

    iomanagerMutex.lock();

    iomanager->gestureInProgressUpdate( eventData );

    iomanagerMutex.unlock();

    prevCentroidHand=centroidHand;
    prevCentroidHand2=centroidHand2;

}

void DoubleSwipeManager::onStatusFinish(FrameData &params)
{
    line( params.rgbDebug, Point2i( 1, 10 ), Point2i( params.rgbDebug.cols-1, 10 ),Scalar(0,255,0),4 );

    Point2i cursorCoordsKinect= (prevCentroidHand2 + prevCentroidHand);
    cursorCoordsKinect.x/=2;
    cursorCoordsKinect.y/=2;

    EventData eventData;
    eventData.mousePosition=cursorCoordsKinect;
    eventData.frameData=&params;
    eventData.offset=0;

    iomanagerMutex.lock();

    iomanager->gestureFinalize( eventData );

    iomanagerMutex.unlock();

}

void DoubleSwipeManager::onStatusInactive(FrameData &params)
{

}

