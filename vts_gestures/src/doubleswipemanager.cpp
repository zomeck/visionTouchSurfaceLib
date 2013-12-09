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


/*
void DoubleSwipeManager::updateStatus( FrameData &params)
{

    bool movementDetected= params.contourHandsFiltered->size() == 2 ;//probably should add that both have fingertips...


    if( movementDetected ){

        resetEndCounter();
        decreaseInitCounter();

    }else{
        resetInitCounter();

        decreaseEndCounter();
    }

    Point2i centroidHand,centroidHand2;

    if( movementDetected ){

        Rect bb=boundingRect( params.contourHandsFiltered->at(0) );
        Rect bb2=boundingRect( params.contourHandsFiltered->at(1) );

        centroidHand.x=bb.x+bb.width/2;
        centroidHand.y=bb.y+bb.height/2;
        params.kinect2screen->mapPoints( centroidHand, centroidHand );

        centroidHand2.x=bb2.x+bb2.width/2;
        centroidHand2.y=bb2.y+bb2.height/2;
        params.kinect2screen->mapPoints( centroidHand2, centroidHand2 );

    }

    if( state==STATUS_INIT ){

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
        iomanager->gestureInitialize( eventData );

        state=STATUS_ACTIVE;
    }else if( state==STATUS_ACTIVE ){

        if( !movementDetected ){
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

        iomanager->gestureInProgressUpdate( eventData );
        prevCentroidHand=centroidHand;
        prevCentroidHand2=centroidHand2;

    }else if( state==STATUS_FINISH ){


        Point2i cursorCoordsKinect= (prevCentroidHand2 + prevCentroidHand);
        cursorCoordsKinect.x/=2;
        cursorCoordsKinect.y/=2;

        EventData eventData;
        eventData.mousePosition=cursorCoordsKinect;
        eventData.frameData=&params;
        eventData.offset=0;
        iomanager->gestureFinalize( eventData );

        state=STATUS_INACTIVE;
    }


    //
    /*
    if( params.contourHandsFiltered->size() == 2 ){


        if( prevCentroidHand2.x==-1 ){
            //initialization
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
            iomanager->gestureInitialize( eventData );

        }else{
            //progress

            Point2i distCentroids=centroidHand-prevCentroidHand;
            Point2i tmpDistMixed=centroidHand-prevCentroidHand2;
            //they are mapped the other way round
            if( cv::norm( tmpDistMixed )< cv::norm( distCentroids ) ){

                Point2i tmp;
                tmp=centroidHand;
                centroidHand=centroidHand2;
                centroidHand2=tmp;
                //distCentroids=centroidHand-prevCentroidHand;
            }

            //circle( drawingHand, centroidHand, 5, CV_RGB(255,0,0),2 );
            //circle( drawingHand, centroidHand2, 5, CV_RGB(0,255,0),2 );

            //imshow("test", drawingHand);
            //cout<< centroidHand <<endl;

            //Point2i distCentroids2=centroidHand2-prevCentroidHand2;
            //now points_2 match with prevPoints_2

            //bool handHasMoved = abs(centroidHand.x-prevCentroidHand.x)>minDist || abs(centroidHand.y-prevCentroidHand.y)>minDist ;
            //bool hand2HasMoved = abs(centroidHand2.x-prevCentroidHand2.x)>minDist || abs(centroidHand2.y-prevCentroidHand2.y)>minDist;

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

                    /*
                    //cout<< zoomFactor <<endl;
                    int button;
                    if(  zoomFactor>0 ){
                        button=4;
                    }else{
                        button=5;
                    }

                    sampleMouseMove( cursorCoordsKinect.x,  cursorCoordsKinect.y );
                    //sampleMouseMove( centroidHand.x,  centroidHand.y );
                    //cout<<cursorCoordsKinect<<endl;

                    XTestFakeButtonEvent (dpy, button, True,  CurrentTime);
                    XFlush(dpy);
                    XTestFakeButtonEvent (dpy, button, False, CurrentTime);
                    XFlush(dpy);



                    //TODO: set data into eventData


                    eventData.offset=zoomFactor;

                }

                eventData.mousePosition=cursorCoordsKinect;

            }else{
                Point2i cursorCoordsKinect=(prevCentroidHand2 + prevCentroidHand);
                cursorCoordsKinect.x/=2;
                cursorCoordsKinect.y/=2;
                eventData.mousePosition=cursorCoordsKinect;
            }

            iomanager->gestureInProgressUpdate( eventData );
            prevCentroidHand=centroidHand;
            prevCentroidHand2=centroidHand2;
        }


    }else{

        Point2i cursorCoordsKinect= (prevCentroidHand2 + prevCentroidHand);
        cursorCoordsKinect.x/=2;
        cursorCoordsKinect.y/=2;

        EventData eventData;
        eventData.mousePosition=cursorCoordsKinect;
        eventData.frameData=&params;
        eventData.offset=0;
        iomanager->gestureFinalize( eventData );

        //ending
        if( prevCentroidHand2.x!=-1 ){

            //XTestFakeButtonEvent (dpy, 1, false,  CurrentTime);
            //XFlush(dpy);
            //cout<<"Unclick"<<endl;
            //prevCentroidHand.x=-1;
            prevCentroidHand2.x=-1;
        }


    }
    */

//}
