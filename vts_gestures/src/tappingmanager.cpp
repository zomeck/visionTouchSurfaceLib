#include "tappingmanager.h"

TappingManager::TappingManager(): GestureManager()
{
    //state=-2;
    //state=STATUS_INACTIVE;
    setCounterInitMax(3);
    resetInitCounter();
    setCounterEndMax(3);
    resetEndCounter();

    minXOffset=8;
    minYOffset=22;
}

void TappingManager::onStatusInactive(FrameData &params)
{
    //nothing
}

void TappingManager::onStatusFinish(FrameData &params)
{
    line( params.rgbDebug, Point2i( 1, 2 ), Point2i( params.rgbDebug.cols-1, 2 ),Scalar(255,0,0),2 );

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

    line( params.rgbDebug, Point2i( 1, 2 ), Point2i( params.rgbDebug.cols-1, 2 ),Scalar(255,0,0),2 );

    Point2i kmCMapped;
    if( gestureDetected ){
        kmCMapped=kmMouse.predictUpdateUsingObs( cMapped );
    }else{
        kmCMapped=kmMouse.predictWithoutObs();
    }

    float offset=std::abs( kmCMapped.x - prevCMapped.x );

    bool hasMoved= offset>minXOffset || std::abs( kmCMapped.y - prevCMapped.y)>minYOffset ;
    //cout<<"TAPPING progress "<<kmCMapped<<endl;
    EventData eventParam;
    eventParam.frameData=&params;
    if( hasMoved ){
        eventParam.mousePosition=kmCMapped;
        prevCMapped=kmCMapped;
        eventParam.offset=offset;
    }else{
        eventParam.mousePosition=prevCMapped;
        eventParam.offset=0;
    }


    iomanagerMutex.lock();

    iomanager->gestureInProgressUpdate( eventParam );

    iomanagerMutex.unlock();


}

void TappingManager::onStatusInit(FrameData &params)
{

    line( params.rgbDebug, Point2i( 1, 2 ), Point2i( params.rgbDebug.cols-1, 2 ),Scalar(255,0,0),4 );
    //initializing gesture
    //cout<<"TAPPING init"<<endl;
    if( !gestureDetected ){
        cout<<"Tapping: WARNING this should not be happening"<<endl;
    }

    prevCMapped=cMapped;

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

        /*
        if( params.matchPointable.x!=-1 ){
            cMapped.x=params.matchPointable.x;
            cMapped.y=params.matchPointable.y;

            cout<<"Using leap information"<<endl;
        }else{
        */
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
        Point2f centerTip=fBB.center;


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


        Point2f midPoint;
        midPoint.x=( vertices[idxMax].x + vertices[idx2ndMax].x )/2;
        midPoint.y=( vertices[idxMax].y + vertices[idx2ndMax].y )/2;

        Point2f vect=midPoint-centerTip;
        vect.x/=norm(vect);
        vect.y/=norm(vect);

        //center.x=( vertices[idxMax].x + vertices[idx2ndMax].x )/2;
        //center.y=( vertices[idxMax].y + vertices[idx2ndMax].y )/2;

        center.x=midPoint.x + vect.x;
        center.y=midPoint.y + vect.y;

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

//}

}

bool TappingManager::isGestureDetected(FrameData &params)
{

    //bool movementDetected=params.contourHandsFiltered->size()==1 && params.contoursFingersHands->at(0).size()==1 && !params.foundPalmHands->at(0);
    bool movementDetected=params.hands->size() ==1 && params.hands->at(0).fingers.size()==1 && !params.hands->at(0).foundPalm;
    return movementDetected;
}
