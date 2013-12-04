#include "tappingmanager.h"

TappingManager::TappingManager(): GestureManager()
{
    //state=-2;
    //state=STATUS_INACTIVE;
    setCounterInitMax(3);
    resetInitCounter();;
}

void TappingManager::onStatusInactive(FrameData &params)
{
//nothing
}

void TappingManager::onStatusFinish(FrameData &params)
{
    Point2i kmCMapped=kmMouse.predictWithoutObs();

    //cout<<"TAPPING end "<<kmCMapped <<endl;

    EventData eventParam;
    eventParam.frameData=&params;
    eventParam.mousePosition=kmCMapped;
    eventParam.offset=0;

    iomanagerMutex.lock();

    iomanager->gestureFinalize( eventParam );

    iomanagerMutex.unlock();


}

void TappingManager::onStatusActive(FrameData &params)
{

    Point2i kmCMapped;
    if( gestureDetected ){
        kmCMapped=kmMouse.predictUpdateUsingObs( cMapped );
    }else{
        kmCMapped=kmMouse.predictWithoutObs();
    }

    //cout<<"TAPPING progress "<<kmCMapped<<endl;
    EventData eventParam;
    eventParam.frameData=&params;
    eventParam.mousePosition=kmCMapped;
    eventParam.offset=0;

    iomanagerMutex.lock();

    iomanager->gestureInProgressUpdate( eventParam );

    iomanagerMutex.unlock();
}

void TappingManager::onStatusInit(FrameData &params)
{

    //initializing gesture
    //cout<<"TAPPING init"<<endl;
    if( !gestureDetected ){
        cout<<"WARNING this should not be happening"<<endl;
    }

    kmMouse.initialiseGesture( cMapped,1 );

    EventData eventParam;
    eventParam.frameData=&params;
    eventParam.mousePosition=cMapped;
    eventParam.offset=0;

    //cout<<"TAPPING init "<<cMapped<<endl;

    iomanagerMutex.lock();

    iomanager->gestureInitialize( eventParam );

    iomanagerMutex.unlock();
    //state=0;

}

void TappingManager::generateGestureData(FrameData &params)
{

    Point2i center;
    if( gestureDetected ){


        if( params.matchPointable.x!=-1 ){
            cMapped.x=params.matchPointable.x;
            cMapped.y=params.matchPointable.y;

            cout<<"Using leap information"<<endl;
        }else{

            //Rect fBB=boundingRect( params.hands->at(0).fingers.at(0).contour );
            //center.x=fBB.tl().x+fBB.width/2;
            //center.y=fBB.br().y;
/*
            RotatedRect fBB=minAreaRect( params.hands->at(0).fingers.at(0).contour );
            center.x=fBB.center.x;
            center.y=fBB.center.y;



            Point2f rect_points[4]; fBB.points( rect_points );
            for( int j = 0; j < 4; j++ ){
                line( draw, rect_points[j], rect_points[(j+1)%4], Scalar(0,255,0), 2, 8 );
            }
*/

            Point2f cHand=params.hands->at(0).centroid;
            RotatedRect fBB=minAreaRect( params.hands->at(0).fingers.at(0).contour );


            Point2f vertices[4];
            fBB.points(vertices);

            int idxMax=-1;
            float distMax=0;
            for( int i=0; i<4; i++ ){
                float dist=norm( vertices[i]-cHand );

                if( dist>distMax ){
                    distMax=dist;
                    idxMax=i;
                }

            }
            int idx2ndMax=-1;
            distMax=0;
            for( int i=0; i<4; i++ ){
                if( i!=idxMax ){
                    float dist=norm(vertices[i]-cHand);
                    if( dist>distMax ){
                        distMax=dist;
                        idx2ndMax=i;
                    }
                }
            }

            //idxMax and idx2ndMax contain the furthest points from the centroid ( even if both distances are equal )



            center.x=( vertices[idxMax].x + vertices[idx2ndMax].x )/2;
            center.y=( vertices[idxMax].y + vertices[idx2ndMax].y )/2;

            /*
            Mat draw=Mat::zeros( params.depthMap.size(),CV_8UC3 );

            for( int j = 0; j < 4; j++ ){
                line( draw, vertices[j], vertices[(j+1)%4], Scalar(0,255,0), 2, 8 );
            }
            circle( draw, center, 2,Scalar(0,0,255),2 );
            imshow("tappingWindow",draw );
            */

            params.kinect2screen->mapPoints( center, cMapped );
        }

    }

}

bool TappingManager::isGestureDetected(FrameData &params)
{

    //bool movementDetected=params.contourHandsFiltered->size()==1 && params.contoursFingersHands->at(0).size()==1 && !params.foundPalmHands->at(0);
    bool movementDetected=params.hands->size() ==1 && params.hands->at(0).fingers.size()==1 && !params.hands->at(0).foundPalm;
    return movementDetected;
}

/*
void TappingManager::updateStatus(FrameData &params)
{
    //if( !isDrag && contoursFFiltered.size()==1 ){
    bool movementDetected=params.contourHandsFiltered->size()==1 && params.contoursFingersHands->at(0).size()==1 && !params.foundPalmHands->at(0);

    if( movementDetected ){

        resetEndCounter();
        decreaseInitCounter();

    }else{
        resetInitCounter();

        decreaseEndCounter();
    }




    //if( movementDetected ){


    //drawContours( drawingHand, contoursFFiltered, 0, CV_RGB(0,255,0) , 2 );

    Point2i cMapped,center;
    if( movementDetected ){
        Rect fBB=boundingRect( params.contoursFingersHands->at(0).at(0) );

        center.x=fBB.tl().x+fBB.width/2;
        center.y=fBB.br().y;
        params.kinect2screen->mapPoints( center, cMapped );
    }

    if( state==STATUS_INIT ){

        //initializing gesture
        //cout<<"TAPPING init"<<endl;
        if( !movementDetected ){
            cout<<"WARNING this should not be happening"<<endl;
        }

        kmMouse.initialiseGesture( cMapped,1 );

        EventData eventParam;
        eventParam.frameData=&params;
        eventParam.mousePosition=cMapped;
        eventParam.offset=0;

        cout<<"TAPPING init "<<cMapped<<endl;

        iomanager->gestureInitialize( eventParam );
        //state=0;

        //initialized now change to active
        state=STATUS_ACTIVE;
    }else if( state==STATUS_ACTIVE ){
        //progress


        Point2i kmCMapped;
        if( movementDetected ){
            kmCMapped=kmMouse.predictUpdateUsingObs( cMapped );
        }else{
            kmCMapped=kmMouse.predictWithoutObs();
        }

        cout<<"TAPPING progress "<<kmCMapped<<endl;
        EventData eventParam;
        eventParam.frameData=&params;
        eventParam.mousePosition=kmCMapped;
        eventParam.offset=0;

        iomanager->gestureInProgressUpdate( eventParam );

        /*
            if( !clickDone ){
                Rect fBB=boundingRect( contoursFFiltered.at(0) );
                Point2i cMapped,center;
                center.x=fBB.tl().x+fBB.width/2;
                center.y=fBB.br().y;
                kinect2screen.mapPoints( center, cMapped );
                sampleMouseMove( cMapped.x, cMapped.y );
                XTestFakeButtonEvent (dpy, 1, true,  CurrentTime);
                XFlush(dpy);
                XTestFakeButtonEvent (dpy, 1, false,  CurrentTime);
                XFlush(dpy);
                clickDone=true;

            }


    }else if( state==STATUS_FINISH ){




        Point2i kmCMapped=kmMouse.predictWithoutObs();

        cout<<"TAPPING end "<<kmCMapped <<endl;

        EventData eventParam;
        eventParam.frameData=&params;
        eventParam.mousePosition=kmCMapped;
        eventParam.offset=0;

        iomanager->gestureFinalize( eventParam );

        //finished now go to inactive
        state=STATUS_INACTIVE;
    }

    //}
    /*else{

        if( state==0 ){
            //transition, should use a counter in the class

            state=1;
        }else if( state==1 ){
            //finish gesture
            cout<<"TAPPING end"<<endl;

            Point2i kmCMapped=kmMouse.predictWithoutObs();

            EventData eventParam;
            eventParam.frameData=&params;
            eventParam.mousePosition=kmCMapped;
            eventParam.offset=0;

            iomanager->gestureFinalize( eventParam );

            state=-2;

            //clickDone=false;
        }

    }

}
*/

